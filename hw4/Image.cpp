// By Kenny Ly and Ryan Bui
#include "Image.h"

Image::Image(int height, int width) {
	this->image = std::vector<std::vector<vec3>>(height, std::vector<vec3>(width));
}

Image::~Image() {

}