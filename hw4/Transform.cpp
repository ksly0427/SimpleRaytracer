// Transform.cpp: implementation of the Transform class.
// Adapted from CSE167 hw2 by Prof. Ravi Ramamoorthi

// By Kenny Ly and Ryan Bui

#include "Transform.h"

// Helper rotation function. 
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    mat3 ret;
    float radians = degrees * (pi / 180);
    glm::vec3 a = glm::normalize(axis);
    float scale = (1.0f - cos(radians));
    float scale2 = sin(radians);
    glm::mat3 outerProduct = glm::mat3(
        scale * a.x * a.x, scale * a.x * a.y, scale * a.x * a.z,
        scale * a.x * a.y, scale * a.y * a.y, scale * a.y * a.z,
        scale * a.x * a.z, scale * a.y * a.z, scale * a.z * a.z);
    glm::mat3 aX = glm::mat3(
        0.0f, scale2 * a.z, scale2 * -a.y,
        scale2 * -a.z, 0.0f, scale2 * a.x,
        scale2 * a.y, scale2 * -a.x, 0.0f);
    ret = glm::mat3(cos(radians), 0.0f, 0.0f, 0.0f, cos(radians), 0.0f, 0.0f, 0.0f, cos(radians)) + outerProduct + aX;
    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
    glm::vec3 a = glm::cross(eye, up);
    up = glm::normalize(glm::cross(a, eye));
    eye = rotate(degrees, up) * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
    glm::vec3 a = glm::cross(eye, up);
    up = glm::normalize(glm::cross(a, eye));
    eye = rotate(degrees, a) * eye;
}

mat4 Transform::lookAt(const vec3& eye, const vec3& center, const vec3& up)
{
    mat4 ret;
    glm::vec3 c = glm::normalize(eye - center);
    glm::vec3 a = glm::normalize(glm::cross(up, c));
    glm::vec3 b = glm::cross(c, a);
    glm::mat4 firstMatrix = glm::mat4(a.x, b.x, c.x, 0.0f, a.y, b.y, c.y, 0.0f, a.z, b.z, c.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 secondMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -eye.x, -eye.y, -eye.z, 1.0f);

    return firstMatrix * secondMatrix;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    float thetaRad = (fovy / 2) * (pi / 180.0f);
    float d = cos(thetaRad) / sin(thetaRad);
    float div = d / aspect;
    float A = -1.0 * ((zFar + zNear) / (zFar - zNear));
    float B = -1.0 * ((2 * zFar * zNear) / (zFar - zNear));
    ret = glm::mat4(div, 0.0f, 0.0f, 0.0f, 0.0f, d, 0.0f, 0.0f, 0.0f, 0.0f, A, -1.0f, 0.0f, 0.0f, B, 0.0f);
    return ret;
}

mat4 Transform::scale(const float& sx, const float& sy, const float& sz)
{
    mat4 ret;
    ret = glm::mat4(sx, 0.0f, 0.0f, 0.0f,
        0.0f, sy, 0.0f, 0.0f,
        0.0f, 0.0f, sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    return ret;
}

mat4 Transform::translate(const float& tx, const float& ty, const float& tz)
{
    mat4 ret;
    ret = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        tx, ty, tz, 1.0f);
    return ret;
} 

vec3 Transform::upvector(const vec3& up, const vec3& zvec)
{
    vec3 x = glm::cross(up, zvec);
    vec3 y = glm::cross(zvec, x);
    vec3 ret = glm::normalize(y);
    return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
