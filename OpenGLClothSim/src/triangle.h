#pragma once
#include <glad/glad.h>
#include "ErrorManager.h"

#include <IL/il.h>		// include DevIL to load images

#include "Particle.h"
#include "Shader.h"


struct VertexTex
{
	glm::vec3 pos;
	glm::vec2 texCoord;
	
};

struct Vertex
{
	glm::vec3 pos;

};

class Triangle {
private:
	unsigned int VBO;
	unsigned int VAO;
	GLuint textID;
public:
	Triangle(Particle &p1, Particle &p2, Particle &p3);
	virtual ~Triangle();

	Vertex triangle[3];
	VertexTex triangleT[3];

	void initialise();
	void initialiseTexture(Shader &myShader);
	bool loadTextureOpenGL(GLuint* data, GLuint width, GLuint height);

	void initTexture();

	void drawLineLoop();
	void drawTexture();

	Particle p[3];

};