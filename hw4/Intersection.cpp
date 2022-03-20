// By Kenny Ly and Ryan Bui
#include "Intersection.h"

Intersection::Intersection() {
	this->dist = -1;
	this->objIndex = 0;
	this->norm = vec3();
	this->pos = vec3();
	this->ray = Ray();
}

Intersection::~Intersection() {

}