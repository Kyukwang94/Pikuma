#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    }
    else {
        this->invMass = 0.0;
    }
    std::cout << "Particle constructor called!" << std::endl;
}
Particle::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}
void Particle::AddForces(const Vec2& force)
{
    sumForces += force;

}
void Particle::ClearForces()
{
    sumForces = Vec2(0.0, 0.0);
}
 
void Particle::Integrate(float dt)
{
    acceleration = sumForces * invMass;

    velocity += acceleration * dt;

    position += velocity * dt;

    ClearForces();
}