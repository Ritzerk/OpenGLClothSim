#pragma once
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/fast_square_root.hpp"



class Particle {
public:
	Particle();
	Particle(glm::vec3 pos, float mass, float damping, glm::vec2 texCoord);
	Particle(glm::vec3 pos);
	Particle(glm::vec3 pos, glm::vec2 texCoord);
	virtual ~Particle();

	bool isStatic;
	glm::vec3 acceleration;
	glm::vec3 pos;
	glm::vec3 lastPos;
	glm::vec2 texCoord;

	float damping;

	float mass;
	glm::vec3 force;

	void makeStatic();
	void move(glm::vec3 delta);
	void addForce(glm::vec3 force);
	void calcMovement(float timeStepSize);

	glm::vec3& getPos();

	friend std::ostream& operator<<(std::ostream& os, const Particle& particle);			//enable cout to output a particle position

};
