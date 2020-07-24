#pragma once

#define NUM_PARTICLES 10

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class ParticleSystem 
{
private:
    glm::vec3   m_posX[NUM_PARTICLES];    // Current positions
    glm::vec3   m_oldPosX[NUM_PARTICLES]; // Previous positions
    glm::vec3   m_acc[NUM_PARTICLES];    // Force accumulators
    glm::vec3   m_vGravity;            // Gravity
    float       m_fTimeStep;

    void       Verlet();                // Verlet integration step
    void       SatisfyConstraints();    // Here constraints should be satisfied
    void       AccumulateForces();      // This function should accumulate forces for each particle

public:
    ParticleSystem(glm::vec3 acc[NUM_PARTICLES], glm::vec3 posX[NUM_PARTICLES], glm::vec3 oldPosX[NUM_PARTICLES]);
    ~ParticleSystem();
    void       TimeStep();
};
