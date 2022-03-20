#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 vec3;

class Ray
{
public:
	Ray();
	Ray(vec3 pos, vec3 dir);
	virtual ~Ray();
	vec3 pos;
	vec3 dir;
	float t_min, t_max;
};

