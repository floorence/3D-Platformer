#include "Mass.h"
#include "Log.h"
#include <glm/gtx/vector_angle.hpp>

void Mass::applyForce(glm::vec3 f) {
    acceleration += f / mass;
}

void Mass::updatePosition(float dt) {
    applyForce(getAirResistance());
    velocity += acceleration * dt;
    processVelocity();
    position += velocity * dt;
}

std::string Mass::getDebugString() {
    return Log::oss("position: ", position.x, ", ", position.y, ", ", position.z, "\n"
        "velocity: ", velocity.x, ", ", velocity.y, ", ", velocity.z, "\n",
        "speed: ", glm::length(velocity), "\n",
        "acceleration: ", acceleration.x, ", ", acceleration.y, ", ", acceleration.z
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