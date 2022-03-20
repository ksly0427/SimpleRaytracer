#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 vec3;



class Camera
{
public:
	Camera();
	virtual ~Camera();
	vec3 eye;
	vec3 W;
	vec3 u;
	vec3 v;
	float fovxr;
	float fovyr;
};

