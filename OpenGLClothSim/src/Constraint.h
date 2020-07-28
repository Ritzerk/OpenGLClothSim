#pragma once

#include "Particle.h"

#include "ErrorManager.h";

class Constraint
{
private:
    float restDistance; // the length between particle p1 and p2 in rest configuration

public:
    Particle p1, p2; // the two particles that are connected through this constraint

    Constraint(Particle &p1, Particle &p2);

    /* This is one of the important methods, where a single constraint between two particles p1and p2 is solved
        the method is called by Cloth.time_step() many times per frame */

    void satisfyConstraint();
    /*{
        glm::vec3 p1_to_p2 = p2->getPos() - p1->getPos(); // vector from p1 to p2
        float current_distance = p1_to_p2.length(); // current distance between p1 and p2
        glm::vec3 correctionVector = p1_to_p2(1 - rest_distance / current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
        glm::vec3 correctionVectorHalf = correctionVector * 0.5; // Lets make it half that length, so that we can move BOTH p1 and p2.
        //p1->offsetPos(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
        //p2->offsetPos(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.
    }*/


};