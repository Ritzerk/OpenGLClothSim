#pragma once

#include <math.h>
#include <iostream>
#include <vector>

#include "ErrorManager.h"
#include "Particle.h"
#include "triangle.h"
#include "Constraint.h"

class Cloth 
{
private:
	int width;
	int height;

	int noOfParticlesWidth, noOfParticlesHeight;

	GLuint VBO, VAO, EBO;
	GLuint textureID;

	

	//Create the particles - this is what we give to the VBO
	std::vector<Particle> particlesVBO;
	std::vector<VertexTex> particlesVBOf;

	std::vector<Constraint> constraints;


	//Create the layout of the triangles - this is what we give to the EBO
	std::vector<unsigned int> particlesEBO; 


public:
	Cloth(float width, float height, float particleSepDistance);
	Cloth(float width, float height, int noOfParticlesW, int noOfParticlesH);
	~Cloth();

	void createEBOvector();
	void createSpringModel();

	void initVBO_EBO_VAO();
	void initTexture(std::string filename);
	bool loadTxtOpenGL(GLuint* data, GLuint width, GLuint height);


	int getIndexAt(int x, int y);
	void createConstraint(Particle *p1, Particle *p2);

	void addGravity(const glm::vec3 direction);
	void addWind(const glm::vec3 direction);

	void detPositioning(float timeStepSize);
	void drawCloth(float timeStepSize);
	
	void updateVBOf();

	void printIntVector(std::vector<GLuint> vec);
	void printParticleVector(std::vector<Particle> vec);
	void normalizeVector(std::vector<Particle> pVector);



	//temporary functions?
	glm::vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);
	void triangleWindForce(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 direction);
	

};