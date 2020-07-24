//glm::mat4 transform = glm::translate(glm::vec3(-50, 20, 25)) * glm::rotate(30.f, glm::vec3(1, 0, 0));
//cloth = new Cloth(100, 50, 100, 50, 100, 0.01, transform);


#include "Cloth.h"



Cloth::Cloth(float width, float height, float particleSepDistance)				//0.8, 0.4, 0.1
{
	int noOfParticlesWidth  = round(width / particleSepDistance);
	int noOfParticlesHeight = round(height / particleSepDistance);


	int noOfParticlesPosNegX = noOfParticlesWidth / 2;
	int noOfParticlesPosNegY = noOfParticlesHeight / 2;

	//Assure the correctness of height and width according to how many particles actually fit. 
	noOfParticlesWidth = noOfParticlesPosNegX*2;
	noOfParticlesHeight = noOfParticlesPosNegY*2;

	std::cout << "noOfParticlesWidth: " << noOfParticlesWidth << "    NoOfParticlesPosNegX: " << noOfParticlesPosNegX << std::endl;
	std::cout << "noOfParticlesHeight: " << noOfParticlesHeight << "    NoOfParticlesPosNegY: " << noOfParticlesPosNegY << std::endl;


	//Creating the VBO vector of particles - to be convertex to array later
	for (int y = -noOfParticlesPosNegY; y <= noOfParticlesPosNegY; y++)
	{
		for (int x = -noOfParticlesPosNegX; x <= noOfParticlesPosNegX; x++) {
			Particle p(glm::vec3(x * particleSepDistance, -y*particleSepDistance, 0.0f));
			particlesVBO.push_back(p);
		}
	}
	
	printParticleVector(particlesVBO);	//Check for accuracy

	

	//now have to put the indexes into index buffer - vector that is to be converted to array so that it is easy to work with for now.
	int firstElementIndexRow2;
	int row1 = 0;
	int row2 = noOfParticlesWidth + 1;		

	for (int topRowsNo = 0; topRowsNo < noOfParticlesHeight; topRowsNo++) {			//iterate through the number of top rows of the strip, one less than total rows as we only need to draw triangle that has the bottom indicie.
		firstElementIndexRow2 = row2;
		for (int nrPerRow = 0; nrPerRow <= noOfParticlesWidth; nrPerRow++) {
			particlesEBO.push_back(row1); //add the first row indexes
			particlesEBO.push_back(row2);	//add the second row indexes
			row1++;
			row2++;
		}
		if (topRowsNo < noOfParticlesHeight-1) {		//don't need to have another degenerate if there isn't another triangle strip starting from the bottom indicies. 
			particlesEBO.push_back(row2-1);
			particlesEBO.push_back(firstElementIndexRow2);
		}
	}

	printIntVector(particlesEBO);	//Check for accuracy	

	createVBO_EBO_VAO();
	
}

void Cloth::createVBO_EBO_VAO() 
{
	//first create particlesVBOf which changes data from a particle to vertex, which we need...
	for (int i = 0; i < particlesVBO.size(); i++) {
		Vertex pos = { particlesVBO[i].pos };   //Create the vertex
		particlesVBOf.push_back(pos);			//Place it into our VBO buffer    
	}

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &EBO));
	GLCall(glGenBuffers(1, &VBO));

	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, particlesVBOf.size()*sizeof(Vertex), &particlesVBOf[0], GL_DYNAMIC_DRAW));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, particlesEBO.size()*sizeof(Vertex), &particlesEBO[0], GL_DYNAMIC_DRAW);

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
}

void Cloth::onUpdate() 
{
	//Updating particlesVBO and particles VBOf here
	for (int i = 0; i < particlesVBO.size(); i++) {
		Vertex pos = { particlesVBO[i].pos };   //Create the vertex
		particlesVBOf[i] = pos;			//Place it into our VBO buffer    
	}
}

void Cloth::initTexture(Shader& myShader)
{
	bool textureLoaded = false;


	ILuint imageID;
	ilGenImages(1, &imageID);	//generate an image name
	ilBindImage(imageID);		//bind it

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	std::string filename = "./res/Red_velvet_pxr128.tif";
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

	myShader.use();
	myShader.setInt("texture1", 0);


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



void Cloth::drawCloth()
{
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLE_STRIP, particlesEBO.size(), GL_UNSIGNED_INT, 0 ));
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

	std::cout << "There are a total of " << vec.size() << " particles along this row. " << std::endl;
	for (std::vector<Particle>::const_iterator i = vec.begin(); i != vec.end(); ++i) {
		std::cout << *i << ", ";
		if (c++ % 5 == 0 && c != 1)
			std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
}

Cloth::~Cloth() {

}