#include "Constraint.h"
#include "glm/ext.hpp"



Constraint::Constraint(Particle &p1, Particle &p2)
    : p1(p1), p2(p2)
{
    restDistance = glm::distance(p1.getPos(), p2.getPos());    //returns the distance between the two vectors in the cloth
    std::cout << "restDistance is: " << restDistance << std::endl;

}

void Constraint::satisfyConstraint()
{
    float currentDistance = glm::distance(p1.getPos(), p2.getPos());   //get the current distance the two particles are apart
    glm::vec3 vectorP1ToP2 = p2.getPos() - p1.getPos();            //the vector that links p1 and p2, in such a way that it is the vector p1 needs to move by in direction of p2 to get to p2. The negative of this vector is what p2 need to move by to get to p1.

    glm::vec3 fixSepDistanceTotal = vectorP1ToP2 * (1.0f - restDistance / currentDistance);                //the total vector p1 and p2 need to be moved by to get to restDistance, but with direction assuming only p1 needs to move to close the gap.
    glm::vec3 fixSepDistanceEach = fixSepDistanceTotal * 0.5f;                                              //each particle needs to move by a half towards each other instead. 

    p1.move(fixSepDistanceEach);
    p1.move(-fixSepDistanceEach);        //this vector needs to be made NEGATIVE as it needs to move toward p1; the original vector's direction assumes direction based on p1 moving towards p2. 

}