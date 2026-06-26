#ifndef MASS_CLASS_H
#define MASS_CLASS_H

#include<glm/glm.hpp>
#include <string>

class Mass {
public: 
    float mass; // kg
	glm::vec3 position; // m
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);	// m/s
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f); // m/s^2

	void applyForce(glm::vec3 f); // DOES NOT actually move the mass. call updatePosition to do so.
    void updatePosition(float dt);
	std::string getDebugString();

    virtual ~Mass() = default;
protected:
	const float AIR_RESISTANCE_COEFFICIENT = 2.0f;
	const float STOPPING_SPEED = 0.0005f;

	glm::vec3 prevAcceleration;

	glm::vec3 getAirResistance(); // return force from air resistance
	virtual void processVelocity();
};

#endif
