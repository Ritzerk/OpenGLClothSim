#include "Particle.h"

Particle::Particle() {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	lastPos = pos;
	mass = 1;
	damping = 0.01;
	//Damping simulates energy loss, and it is used in physics simulations to make sure that
	//springs don’t oscillate forever but come to rest over time.
	texCoord = glm::vec2(0.0f, 0.0f);

	force = glm::vec3(0, 0, 0);
	isStatic = false;

}

Particle::Particle(glm::vec3 pos, float mass, float damping, glm::vec2 texCoord) {	
	this->pos = pos;
	this->lastPos = pos;
	this->mass = mass;
	this->damping = damping; 
	//Damping simulates energy loss, and it is used in physics simulations to make sure that
	//springs don’t oscillate forever but come to rest over time.
	this->texCoord = texCoord;

	force = glm::vec3(0, 0, 0);
	isStatic = false;
}

Particle::Particle(glm::vec3 pos) {
	this->pos = pos;

	this->mass = 1;
	this->damping = 0.1;

	force = glm::vec3(0, 0, 0);
	isStatic = false;
}

Particle::Particle(glm::vec3 pos, glm::vec2 texCoord) 
{
	this->pos = pos;
	this->texCoord = texCoord;

	this->mass = 1;
	this->damping = 0.1;

	force = glm::vec3(0, 0, 0);
	isStatic = false;
}

std::ostream& operator<<(std::ostream& os, const Particle& particle)
{
	os << "{"
		<< particle.pos.x << " " << particle.pos.y << " " << particle.pos.z
		<< "}";
	return os;
}

Particle::~Particle() {
}


void Particle::move(glm::vec3 delta) {
	if (isStatic) return;
	pos += delta;
}

void Particle::step(float timeStep) {
	if (isStatic) return;

	//apply force
	glm::vec3 tempPos = pos;
	pos = pos + (pos - lastPos) * damping + (force / mass) * (timeStep / 100);		//f/m is acceleration.
	lastPos = tempPos;
	force = glm::vec3(0, 0, 0);	//cloth = new Cloth(100, 50, 100, 50, 100, 0.01, transform);
}