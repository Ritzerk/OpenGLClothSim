#pragma once

#include <math.h>
#include <iostream>
#include <vector>

#include "ErrorManager.h"
#include "Particle.h"
#include "triangle.h"

class Cloth 
{
private:
	int width;
	int height;
	GLuint VBO, VAO, EBO;
	GLuint textureID;


	//Create the particles - this is what we give to the VBO
	std::vector<Particle> particlesVBO;
	std::vector<VertexTex> particlesVBOf;


	//Create the layout of the triangles - this is what we give to the EBO
	std::vector<unsigned int> particlesEBO; 


public:
	Cloth(float width, float height, float particleSepDistance);
	~Cloth();

	void createVBO_EBO_VAO();
	void initTexture();
	bool loadTxtOpenGL(GLuint* data, GLuint width, GLuint height);

	void drawCloth();
	void onUpdate();

	void printIntVector(std::vector<GLuint> vec);
	void printParticleVector(std::vector<Particle> vec);
	

};