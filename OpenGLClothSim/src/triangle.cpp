#include "Triangle.h"

Triangle::Triangle(Particle &p1, Particle &p2, Particle &p3) {

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;

	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenVertexArrays(1, &VAO));

}

void Triangle::initialise() {


	for (int i = 0; i < 3; i++) {
		Vertex pos = { p[i].pos };   //Create the vertex
		triangle[i] = pos;		     //Place it into our VBO buffer
	}

	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_DYNAMIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

}

void Triangle::initialiseTexture(Shader &myShader) 
{
	for (int i = 0; i < 3; i++) {
		VertexTex vert = { p[i].pos, p[i].texCoord };   //Create the vertex
		triangleT[i] = vert;								//Place it into our VBO buffer
	}

	GLCall(glBindVertexArray(VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(triangleT), triangleT, GL_DYNAMIC_DRAW));

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	//do the same for texture
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3));
	GLCall(glEnableVertexAttribArray(1));
	
	
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
	} else {
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);		//make sure the pixel data is in RGBA format
		if (success == IL_TRUE)
		{
			textureLoaded = loadTextureOpenGL((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
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
	} else {
		std::cout << "Image loaded\n";
	}

	myShader.use();
	myShader.setInt("texture1", 0);


}

bool Triangle::loadTextureOpenGL(GLuint* data, GLuint width, GLuint height) 
{
	//Generate texture ID
	GLCall(glGenTextures(1, &textID));

	//Bind texture ID
	GLCall(glBindTexture(GL_TEXTURE_2D, textID));
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

void Triangle::drawLineLoop() {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
}


void Triangle::initTexture() 
{
}


void Triangle::drawTexture() 
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, textID));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

Triangle::~Triangle() {
}