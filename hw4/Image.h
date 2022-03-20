#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

typedef glm::vec3 vec3;

class Image
{
public:
	Image(int height, int width);
	virtual ~Image();
	std::vector<std::vector<vec3>> image;
};

