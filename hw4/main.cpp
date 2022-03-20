// main.cpp coordinates the raytracer and renders the image using FreeImage
// By Kenny Ly and Ryan Bui

#include <iostream>
#include "FreeImage.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;

#define MAINPROGRAM
#include "variables.h"
#include "readfile.h"
#include "Ray.h"
#include "Intersection.h"
#include "Camera.h"
#include "Image.h"

Ray rayThruPixel(Camera cam, int i, int j) {
	float alpha = cam.fovxr * ((j + 0.5f) - (w / 2.0f)) / (w / 2.0f);
	float beta = glm::tan(cam.fovyr / 2.0f) * ((((float)h / 2.0f) - (i + 0.5f)) / ((float)h / 2.0f));
	vec3 d = glm::normalize(alpha * cam.u + beta * cam.v - cam.W);
	Ray ray = Ray(cam.eye, d);
	return ray;
}

Intersection intersectObj(Ray ray, object* obj, int objIndex) {
	if (obj->type == sphere) {
		vec3 rayPosTrans = vec3(inverse(obj->transform) * vec4(ray.pos, 1.0f));
		vec3 rayDirTrans = vec3(inverse(obj->transform) * vec4(ray.dir, 0.0f));
		float a = glm::dot(rayDirTrans, rayDirTrans);
		float b = 2.0f * glm::dot(rayDirTrans, (rayPosTrans - obj->center));
		float c = glm::dot(rayPosTrans - obj->center, rayPosTrans - obj->center) - (float)pow(obj->radius, 2);
		float d = pow(b, 2) - 4.0f * a * c;
		float t = -1;
		if (d < 0) { 
			t = -1;
		}
		else {
			float tpos = (-b + sqrt(d)) / (2.0f * a);
			float tneg = (-b - sqrt(d)) / (2.0f * a);
			float tmin = glm::min(tpos, tneg);
			float tmax = glm::max(tpos, tneg);

			if (tmin >= 0) {
				t = tmin;
			}
			else if (tmax >= 0) {
				t = tmax;
			}
			else {
				t = -1;
			}
		}

		if (t >= ray.t_min && t <= ray.t_max) {
			vec3 pt = rayPosTrans + rayDirTrans * t;
			vec3 p = vec3(obj->transform * vec4(pt, 1.0f));
			Intersection temp = Intersection();
			temp.dist = t;
			temp.pos = p;
			vec4 initSphere = vec4(pt - obj->center, 0.0f);
			mat4 inverseTransform = inverse(obj->transform);
			temp.norm = glm::normalize(vec3(transpose(inverseTransform) * initSphere));
			temp.objIndex = objIndex;
			temp.ray = ray;
			return temp;
		}

		Intersection temp = Intersection();
		temp.dist = -1;
		temp.pos = vec3(0.0f);
		temp.norm = vec3(0.0f);
		temp.objIndex = objIndex;
		temp.ray = ray;
		return temp;
	}
	else if (obj->type == tri) {
		vec3 A = vec3(obj->transform * vec4(vertexes[obj->vertices[0]], 1.0f));
		vec3 B = vec3(obj->transform * vec4(vertexes[obj->vertices[1]], 1.0f));
		vec3 C = vec3(obj->transform * vec4(vertexes[obj->vertices[2]], 1.0f));

		vec3 norm = glm::normalize(glm::cross(B - A, C - A));

		if (glm::dot(ray.dir, norm) == 0) {
			Intersection temp = Intersection();
			temp.dist = -1;
			temp.pos = vec3(0.0f);
			temp.norm = norm;
			temp.objIndex = objIndex;
			temp.ray = ray;
			return temp;
		}

		float t = (glm::dot(A, norm) - glm::dot(ray.pos, norm)) / glm::dot(ray.dir, norm);
		vec3 P = ray.pos + ray.dir * t;

		vec3 nCrossBC = glm::cross(norm, C - B);
		vec3 apNorm = nCrossBC / (glm::dot(nCrossBC, A - C));
		float apW = glm::dot(-1.0f * apNorm, C);

		vec3 nCrossCA = glm::cross(norm, A - C);
		vec3 bpNorm = nCrossCA / (glm::dot(nCrossCA, B - A));
		float bpW = glm::dot(-1.0f * bpNorm, A);

		vec3 nCrossAB = glm::cross(norm, B - A);
		vec3 cpNorm = nCrossAB / (glm::dot(nCrossAB, C - B));
		float cpW = glm::dot(-1.0f * cpNorm, B);

		float a = glm::dot(apNorm, P) + apW;
		float b = glm::dot(bpNorm, P) + bpW;
		float c = glm::dot(cpNorm, P) + cpW;

		if (a < 0 || a > 1) {
			t = -1;
		}

		if (b < 0 || b > 1) {
			t = -1;
		}

		if (c < 0 || c > 1) {
			t = -1;
		}

		Intersection temp = Intersection();
		temp.dist = t;
		temp.pos = P;
		temp.norm = norm;
		temp.objIndex = objIndex;
		temp.ray = ray;
		return temp;

	}
}

Intersection intersects(Ray ray) {
	float minDist = INFINITY;
	Intersection hit = Intersection();
	for (int i = 0; i < numobjects; i++) {
		object* obj = &(objects[i]);
		Intersection temp = intersectObj(ray, obj, i);
		temp.objIndex = i;
		if (temp.dist < minDist && temp.dist >= 0) {
			minDist = temp.dist;
			hit = temp;
		}
	}
	return hit;
}

float myDistance(vec3 a, vec3 b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

vec3 myReflect(vec3 a, vec3 b) {
	return a - 2.0f * dot(b, a) * b;
}

vec3 findColor(Intersection hit, int& depth) {
	depth++;
	object* obj = &(objects[hit.objIndex]);
	vec3 intensity = vec3(0.0f, 0.0f, 0.0f);
	if (hit.dist > -1) {
		Ray shadowR = Ray();
		Ray reflectR = Ray();
		float V = 1.0f;
		float denom = 1.0f;

		intensity += obj->ambient + obj->emission;
		for (int i = 0; i < numused; i++) {
			int indexColor = i * 3;
			int indexPos = i * 4;
			vec3 colorLight = vec3(lightcolor[indexColor], lightcolor[indexColor + 1], lightcolor[indexColor + 2]);
			vec3 posLight = vec3(lightposn[indexPos], lightposn[indexPos + 1], lightposn[indexPos + 2]);
			int w = lightposn[indexPos + 3];

			if (w == 1) { // Point
				shadowR.dir = glm::normalize(posLight - hit.pos);
				shadowR.pos = hit.pos + 0.001f * shadowR.dir;
				Intersection shadowInt = intersects(shadowR);
				float aToLight = myDistance(posLight, hit.pos);
				float aToB = shadowInt.dist;
				if (aToB < aToLight && shadowInt.dist >= 0) {
					V = 0.0f;
				}
				denom = attenuation[0] + attenuation[1] * aToLight + attenuation[2] * pow(aToLight, 2);
			}
			else if (w == 0) { // directional
				shadowR.dir = glm::normalize(posLight);
				shadowR.pos = hit.pos + 0.001f * shadowR.dir;
				Intersection shadowInt = intersects(shadowR);
				if (shadowInt.dist >= 0) {
					V = 0.0f;
				}
			}

			vec3 eyeDir = glm::normalize(hit.ray.pos - hit.pos);
			vec3 half = glm::normalize(shadowR.dir + eyeDir);
			float nL = dot(hit.norm, shadowR.dir);
			float nH = dot(hit.norm, half);
			vec3 diffuseT = obj->diffuse * max(nL, 0.0f);
			vec3 specularT = obj->specular * pow(max(nH, 0.0f), obj->shininess);
			intensity += V * (colorLight / denom) * (diffuseT + specularT);
		}
		
		
		reflectR.pos = (hit.pos + 0.001f * hit.norm);
		reflectR.dir = myReflect(hit.ray.dir, hit.norm);
		hit = intersects(reflectR);
			
		while (depth <= maxdepth) {
			intensity += obj->specular * findColor(hit, depth);
		}
		return intensity;
	}
	return vec3(0.0f, 0.0f, 0.0f);
}

Image raytrace(Camera cam, int width, int height) {
	Image image = Image(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float percent = (float(w * i) + float(j)) / float(width * height);
			//cout << percent * 100 << "%\n";
			Ray ray = rayThruPixel(cam, i, j);
			Intersection hit = intersects(ray);
			int depth = 0;
			image.image[i][j] = findColor(hit, depth);
		}
	}
	return image;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n";
		exit(-1);
	}

	FreeImage_Initialise();
	attenuation[0] = 1; attenuation[1] = 0; attenuation[2] = 0;
	ambient[0] = 0.2f; ambient[1] = 0.2f; ambient[2] = 0.2f;
	maxdepth = 5;
	output = "raytrace.png";
	readfile(argv[1]);
	Camera cam = Camera();
	Image image = raytrace(cam, w, h);
	
	FIBITMAP* bits = FreeImage_Allocate(w, h, 24);
	RGBQUAD color;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			color.rgbRed = image.image[h - i - 1][j][0] * 255.0f;
			color.rgbGreen = image.image[h - i - 1][j][1] * 255.0f;
			color.rgbBlue = image.image[h - i - 1][j][2] * 255.0f;
			FreeImage_SetPixelColor(bits, j, i, &color);
		}
	}

	FreeImage_Save(FIF_PNG, bits, output.c_str(), 0);
	FreeImage_DeInitialise();
}
