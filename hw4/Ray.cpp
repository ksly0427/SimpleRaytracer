// By Kenny Ly and Ryan Bui

#include "ray.h"

Ray::Ray() {
	this->pos = vec3(0.0f, 0.0f, 0.0f);
	this->dir = vec3(0.0f, 0.0f, 0.0f);
	this->t_min = 0;
	this->t_max = INFINITY;
}

Ray::Ray(vec3 pos, vec3 dir) {
	this->pos = pos;
	this->dir = dir;
	this->t_min = 0;
	this->t_max = 500;
}

Ray::~Ray() {

}
