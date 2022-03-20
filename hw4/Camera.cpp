// By Kenny Ly and Ryan Bui
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "variables.h"

// glm provides vector, matrix classes like glsl
// Typedefs to make code more readable 

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

Camera::Camera() {
	vec3 a = eyeinit - center;
	vec3 b = upinit;
	this->W = glm::normalize(a);
	this->u = glm::normalize(glm::cross(b, this->W));
	this->v = glm::cross(this->W, this->u);
	this->eye = eyeinit;
	this->fovyr = glm::radians(fovy);
	this->fovxr = glm::tan(fovyr/2.0f) * ((float)w / (float)h);
}

Camera::~Camera() {

}