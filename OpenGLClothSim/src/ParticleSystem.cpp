#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 acc[NUM_PARTICLES], glm::vec3 posX[NUM_PARTICLES], glm::vec3 oldPosX[NUM_PARTICLES])
{
    m_vGravity = 5;            // Gravity
    m_fTimeStep = 5;
}


ParticleSystem::~ParticleSystem() {};

// Verlet integration step
void ParticleSystem::Verlet() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        glm::vec3& posX = m_posX[i];
        glm::vec3 temp = posX;
        glm::vec3& oldPosX = m_oldPosX[i];
        glm::vec3& acc = m_acc[i];
        posX += posX - oldPosX + acc * m_fTimeStep * m_fTimeStep;
        oldPosX = temp;
    }
}
// This function should accumulate forces for each particle
void ParticleSystem::AccumulateForces()
{
    // All particles are influenced by gravity
    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        m_acc[i] = m_vGravity;
    }
}
// Here constraints should be satisfied
void ParticleSystem::SatisfyConstraints() {
    // Ignore this function for now
}
void ParticleSystem::TimeStep() {
    AccumulateForces();
    Verlet();
    SatisfyConstraints();
}