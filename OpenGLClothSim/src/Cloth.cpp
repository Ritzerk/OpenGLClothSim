//glm::mat4 transform = glm::translate(glm::vec3(-50, 20, 25)) * glm::rotate(30.f, glm::vec3(1, 0, 0));
//cloth = new Cloth(100, 50, 100, 50, 100, 0.01, transform);

#include <glm/gtx/normal.hpp>
#include "Cloth.h"



Cloth::Cloth(float width, float height, float particleSepDistance)				//0.8, 0.4, 0.1
{
	noOfParticlesWidth  = round(width / particleSepDistance) + 1; //total number of particles will be one higher as there is a particle at 0 as well.
	noOfParticlesHeight = round(height / particleSepDistance) + 1;


	std::cout << "noOfParticlesWidth: " << noOfParticlesWidth  << std::endl;
	std::cout << "noOfParticlesHeight: " << noOfParticlesHeight << std::endl;

	//Creating the VBO vector of particles
	for (int y = noOfParticlesHeight-1; y >= 0; y--)
	{
		for (int x = 0; x < noOfParticlesWidth; x++) {
			Particle p(glm::vec3(x * particleSepDistance, y*particleSepDistance, 0.0f), glm::vec2((float)x/(noOfParticlesWidth-1), (float)y/(noOfParticlesHeight-1) ));
			//Particle p(glm::vec3(width*(x/(float)noOfParticlesWidth), -height*(y/noOfParticlesHeight), 0.0f), glm::vec2((float)x/(noOfParticlesWidth-1), (float)y/(noOfParticlesHeight-1) ));
			particlesVBO.push_back(p);
		}
	}

	//printParticleVector(particlesVBO);	//Check for accuracy

	//now have to put the indexes into index buffer - vector that is to be converted to array so that it is easy to work with for now.
	createEBOvector();

	//printIntVector(particlesEBO);	//Check for accuracy	

	initVBO_EBO_VAO();
	initTexture("./res/Checked_cotton_pxr128_bmp.tif");

	//Create the structural, shear and bending constraints now. How they are linked can be seen on the sheets.
	createSpringModel();
	
	//make corners unmovable to keep the cloth in stable position on the screen, by making first three and last three particles in the corner unmovable.
	for (int i = 0; i < 3; i++) 
	{
		particlesVBO[i].makeStatic();
		particlesVBO[(noOfParticlesWidth - 1) - i].makeStatic();
	}
}



void Cloth::createEBOvector() {

	int firstElementIndexRow2;
	int row1 = 0;
	int row2 = noOfParticlesWidth;		//the index of the first element of second row is the number of particles of the width, as vector starts at 0

	for (int topRowNo = 0; topRowNo < (noOfParticlesHeight - 1); topRowNo++) {			//iterate through the number of top rows of the strip, one less than total rows as we only need to draw triangle that has the bottom indicie.
		firstElementIndexRow2 = row2;
		for (int nrPerRow = 0; nrPerRow < noOfParticlesWidth; nrPerRow++) {
			particlesEBO.push_back(row1); //add the first row indexes
			particlesEBO.push_back(row2);	//add the second row indexes
			row1++;
			row2++;
		}
		if (topRowNo < (noOfParticlesHeight - 2)) {		//don't need to have another degenerate if there isn't another triangle strip starting from the bottom indicies. 
			//Add the degenerate triangles
			particlesEBO.push_back(row2 - 1);							//add the last element of the row
			particlesEBO.push_back(firstElementIndexRow2);			//add the first element of the row
		}
	}
}

void Cloth::createSpringModel() {

	for (int y = 0; y < noOfParticlesHeight; y++)
	{
		for (int x = 0; x < noOfParticlesWidth; x++)
		{
			/*~~~~~~STRUCTURAL SPRINGS~~~~~~*/

			//Link the particles across the row together 
			if (x < noOfParticlesWidth - 1)   //the last particle isn't to be linked with the particle in the next row. 
			{

				createConstraint(&particlesVBO[getIndexAt(x, y)], &particlesVBO[getIndexAt(x + 1, y)]);
				
			}

			//Link the particles across the column together
			if (y < noOfParticlesHeight - 1) //the last row doesn't have a row below it to link to.
			{
				createConstraint(&particlesVBO[getIndexAt(x, y)], &particlesVBO[getIndexAt(x, y + 1)]);
			}


			/*~~~~~SHEAR SPRINGS~~~~~*/

			if (x < noOfParticlesWidth - 1)
			{
				//Link the particles across diagonally to the right
				if (y < noOfParticlesHeight - 1)		//the last element in a row doesn't have a particle to the lower right to link to, and last row doesn't have a next row to link to.
				{
					createConstraint(&particlesVBO[getIndexAt(x, y)], &particlesVBO[getIndexAt(x + 1, y + 1)]);
				}

				//Link the particles across diagonally to the left
				if (y < noOfParticlesHeight - 1) {		//the first particle doesn't have a particle to lower left, hence we start at x+1 and have to say x goes up to only noOfParticlesWidth - 1. The last row doesn't have a next row to link to.
					createConstraint(&particlesVBO[getIndexAt(x + 1, y)], &particlesVBO[getIndexAt(x, y + 1)]);
					//std::cout << "Index1: " << index1 << "  Index2: " << index2 << std::endl;  //Check what constraints were created
				}
			}


			/*~~~~BEND SPRINGS~~~~*/

			//horizontal bends over particle
			if (x < noOfParticlesWidth - 2) //First bend goes over one particle horizontally to the right, so last two particles in a row don't have any particle to link to
			{
				createConstraint(&particlesVBO[getIndexAt(x, y)], &particlesVBO[getIndexAt(x + 2, y)]);
			}

			//vertical bends over particle
			if (y < noOfParticlesHeight - 2) //First bend goes over one particle diagonally, so last two particles in a column don't have any particle to link to
			{
				createConstraint(&particlesVBO[getIndexAt(x, y)], &particlesVBO[getIndexAt(x, y + 2)]);

			}

			//diagonal bends over a particle
			if (x < noOfParticlesWidth - 2 && y < noOfParticlesHeight - 2)
			{
				//bending diagonally to the right over a particle. Last two in a row have nothing to link with,  and last two in column don't have anything to link with either.
				createConstraint(&particlesVBO[getIndexAt(x, y)], &particlesVBO[getIndexAt(x + 2, y + 2)]);

				//bending diagonally to the left over a particle. First two don't link, since first particle that links is at x+2, we can't go beyond noOfParticlesWidth-2. The particle it links to is at y+2, so have to take away two in height as well.
				createConstraint(&particlesVBO[getIndexAt(x + 2, y)], &particlesVBO[getIndexAt(x, y + 2)]);
			}
		}
	}
}

int Cloth::getIndexAt(int column, int row)
{
	return row * noOfParticlesWidth + column;
}

void Cloth::createConstraint(Particle *p1, Particle *p2)
{
	constraints.push_back(Constraint(p1, p2));

}

void Cloth::initVBO_EBO_VAO() 
{
	//first create particlesVBOf which changes data from a particle to vertex, which we need...
	for (int i = 0; i < particlesVBO.size(); i++) {
		VertexTex vert = { particlesVBO[i].pos, particlesVBO[i].texCoord };   //Create the vertex
		particlesVBOf.push_back(vert);			//Place it into our VBO buffer    
	}

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &EBO));
	GLCall(glGenBuffers(1, &VBO));

	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, particlesVBOf.size()*sizeof(VertexTex), &particlesVBOf[0], GL_STREAM_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, particlesEBO.size()*sizeof(VertexTex), &particlesEBO[0], GL_DYNAMIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	//Do the same for texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

}

void Cloth::updateVBOf() 
{
	//printParticleVector(particlesVBO);

	for (int i = 0; i < particlesVBO.size(); i++) {
		VertexTex vert = { particlesVBO[i].pos, particlesVBO[i].texCoord };   //Create the vertex
		particlesVBOf[i] = vert;			//Place it into our VBO buffer; instead of push_back, now can actually replace the values since the size of particlesVBOf should be the same of that of particlesVBO. 
	}

	//printParticleVector(particlesVBO);


}

//A function to determine the positioning of every particle within the cloth.
void Cloth::detPositioning(float timeStepSize) {
	//first we need to satisfy the constraints in order to determine the positioning. 
	for (int i = 0; i < 15; i++)
	{
		for (std::vector<Constraint>::iterator constraint = constraints.begin(); constraint != constraints.end(); constraint++)
		{
			(*constraint).satisfyConstraint();
		}
	}

	for (std::vector<Particle>::iterator particle = particlesVBO.begin(); particle != particlesVBO.end(); particle++)
	{
		(*particle).calcMovement(timeStepSize);
	}
}

void Cloth::addGravity(const glm::vec3 direction)
{
	for (std::vector<Particle>::iterator particle = particlesVBO.begin(); particle != particlesVBO.end(); particle++)
	{
		(*particle).addForce(direction);
	}
}

glm::vec3 Cloth::calcTriangleNormal(Particle* p1, Particle* p2, Particle* p3)
{
	glm::vec3 pos1 = p1->getPos();
	glm::vec3 pos2 = p2->getPos();
	glm::vec3 pos3 = p3->getPos();

	return glm::cross(pos3 - pos1, pos2 - pos1);
}


void Cloth::triangleWindForce(Particle* p1, Particle* p2, Particle* p3, const glm::vec3 direction)
{
	glm::vec3 normal = calcTriangleNormal(p1, p2, p3);
	glm::vec3 normalized = glm::normalize(normal);
	glm::vec3 force = normal * (glm::dot(normalized, direction));

	p1->addForce(force);
	p2->addForce(force);
	p3->addForce(force);
}

void Cloth::addWind(glm::vec3 direction)
{
	for (int x = 0; x < noOfParticlesWidth - 1; x++)
	{
		for (int y = 0; y < noOfParticlesHeight - 1; y++)
		{
			triangleWindForce(&particlesVBO[getIndexAt(x+1,y)], &particlesVBO[getIndexAt(x, y)], &particlesVBO[getIndexAt(x, y+1)], direction);
			triangleWindForce(&particlesVBO[getIndexAt(x + 1, y+1)], &particlesVBO[getIndexAt(x + 1, y)], &particlesVBO[getIndexAt(x, y+1)], direction);
		}
	}
}

//Method called each frame
void Cloth::drawCloth(float timeStepSize)
{
	addGravity(glm::vec3(0,-0.0002,0)*timeStepSize); //addGravity: add gravity to every particle each frame, pointing down
	//addWind(glm::vec3(0.005,0,0.2)*timeStepSize); //addWind: generate some wind each frame
	detPositioning(timeStepSize); //Calculate the particle positions of the next frame, by resolving the constraints and time step of each particle

	updateVBOf();

	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, particlesVBOf.size() * sizeof(VertexTex), &particlesVBOf[0], GL_STREAM_DRAW));

	GLCall(glDrawElements(GL_TRIANGLE_STRIP, particlesEBO.size(), GL_UNSIGNED_INT, 0 ));
}



void Cloth::initTexture(std::string filename)
{
	bool textureLoaded = false;

	ILuint imageID;
	ilGenImages(1, &imageID);	//generate an image name
	ilBindImage(imageID);		//bind it

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ILboolean success = ilLoadImage((ILstring)filename.c_str());	//load the image

	// check to see if everything went OK
	if (!success) {
		ilDeleteImages(1, &imageID);
		std::cout << "Failed to load image\n";
	}
	else {
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);		//make sure the pixel data is in RGBA format
		if (success == IL_TRUE)
		{
			textureLoaded = loadTxtOpenGL((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
		}
		else
		{
			std::cout << "Couldn't convert image to RGB\n" << std::endl;
		}

		ilDeleteImages(1, &imageID);		//clean image from memory
	}

	//Report Error
	if (!textureLoaded) {
		std::cout << "Unable to load texture.\n";
	}
	else {
		std::cout << "Image loaded\n";
	}

	//myShader.use();
	//myShader.setInt("texture1", 0);


}

bool Cloth::loadTxtOpenGL(GLuint* data, GLuint width, GLuint height)
{
	//Generate texture ID
	GLCall(glGenTextures(1, &textureID));

	//Bind texture ID
	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
	//Generate texture
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	//GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	//Set the texture wrapping/filteing options on the currently bound texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));		//Repeats texture where the object is bigger than texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));	//Linear for downscailing images - the returned colour of pixel is a mix (interpolated) value of colours, if we are downscailing, then we can't show every single pixel on the image inside the object.
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));   //Linear as we don't use mipmap when upscailing. 

	//Unbind texture
	GLCall(glBindTexture(GL_TEXTURE_2D, NULL));

	return true;
}


void Cloth::printIntVector(std::vector<GLuint> vec) {
	int c = 0;
	for (std::vector<GLuint>::const_iterator i = particlesEBO.begin(); i != particlesEBO.end(); ++i) {
		std::cout << *i << ", ";
		if (c++ % 5 == 0 && c != 1)
			std::cout << std::endl;
	}
}

void Cloth::printParticleVector(std::vector<Particle> vec)
{
	int c = 0;
	std::cout << "This is what the current vector contains in this frame: " << std::endl;
	for (std::vector<Particle>::const_iterator i = vec.begin(); i != vec.end(); ++i) {
		std::cout << *i << ", ";
		if (c++ % 5 == 0 && c != 1)
			std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;

}

void Cloth::normalizeVector(std::vector<Particle> pVector)
{
	for (std::vector<Particle>::iterator particle = particlesVBO.begin(); particle != particlesVBO.end(); particle++)
	{
		(*particle).pos = glm::normalize((*particle).getPos());
	}
}

Cloth::~Cloth() {
	//Clear up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

