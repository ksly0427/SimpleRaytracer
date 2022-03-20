#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Ray.h"
#include <string>

typedef glm::vec3 vec3;

class Intersection
{
public:
	Intersection();
	virtual ~Intersection();
	float dist;
	vec3 pos;
	vec3 norm;
	int objIndex;
	Ray ray;

};

