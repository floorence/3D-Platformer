#include "Mass.h"
#include<fmt/format.h>
#include <glm/gtx/vector_angle.hpp>

void Mass::applyForce(glm::vec3 f) {
    acceleration += f / mass;
}

void Mass::updatePosition(float dt) {
    applyForce(getAirResistance());
    velocity += acceleration * dt;
    processVelocity();
    position += velocity * dt;
	acceleration = glm::vec3(0.0f);
}

std::string Mass::getDebugString() {
    return fmt::format("position: {:.4f}, {:.4f}, {:.4f}\nvelocity: {:.4f}, {:.4f}, {:.4f}\nspeed: {:.4f}\nacceleration: {:.4f}, {:.4f}, {:.4f}", 
        position.x, position.y, position.z,
        velocity.x, velocity.y, velocity.z,
        glm::length(velocity),
        acceleration.x, acceleration.y, acceleration.z
    );
}

glm::vec3 Mass::getAirResistance() {
	return -velocity * AIR_RESISTANCE_COEFFICIENT;
}

void Mass::processVelocity() {
    glm::vec3 a = glm::normalize(acceleration);
    glm::vec3 v = glm::normalize(velocity);

    float angleRadians = glm::angle(a, v); 
    float angleDegrees = glm::degrees(angleRadians); 

    // if acceleration is near the same direction as velocity, it is increasing the magnitude of velocity; don't stop
    if (angleDegrees < 90.0f) return;

	if (glm::length(velocity) < STOPPING_SPEED) {
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}