#include "Particle.h"

Particle::Particle() {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	lastPos = pos;
	mass = 1;
	damping = 0.99;
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

glm::vec3& Particle::getPos() 
{
	return pos;
}

Particle::Particle(glm::vec3 pos) {
	this->pos = pos;

	this->mass = 1;
	this->damping = 0.99;

	force = glm::vec3(0, 0, 0);
	isStatic = false;
}

Particle::Particle(glm::vec3 pos, glm::vec2 texCoord) 
{
	this->pos = pos;
	this->lastPos = pos;
	this->texCoord = texCoord;

	this->mass = 1;
	this->damping = 0.99;  //particle damping is 0.99

	force = glm::vec3(0, 0, 0);
	isStatic = false;
}

std::ostream& operator<<(std::ostream& os, const Particle& particle)
{
	os << "{"
		<< particle.pos.x << ", " << particle.pos.y << ", " << particle.pos.z
		<< "}";
	os << "{"
		<< particle.texCoord.x << ", " << particle.texCoord.y
		<< "}";
	return os;
}

Particle::~Particle() {
}

void Particle::makeStatic() 
{
	isStatic = true;
}


void Particle::move(glm::vec3 movementVector) {
	if (isStatic) return;      //if this particle isn't suppose to move, do nothing.
	pos += movementVector;			   //if this particle should move, then go ahead.
}

void Particle::addForce(glm::vec3 force)
{
	acceleration += force/mass;
}

//Calculates the position of the particle on this iteration of graphics
void Particle::calcMovement(float timeStepSize) {
	if (isStatic) return;

	//apply force
	glm::vec3 tempPos = pos;
	pos = pos + (pos - lastPos) * damping + acceleration * (timeStepSize/100);		//  f/m is acceleration. timeStep id dt which is the change in time. 
	lastPos = tempPos;
	acceleration = glm::vec3(0, 0, 0);	//resetting acceleration
}