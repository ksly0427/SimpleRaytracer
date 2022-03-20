/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp adapted from CSE167 Hw2 by Ravi Ramamoorthi.  
// It includes helper functions for matrix  transformations for a stack (matransform) 
// and to rightmultiply the top of a stack.  These functions are given to aid in setting
// up the transformations properly, and to use glm functions in the right way.  

// The functions readvals and readfile do basic parsing.

// By Kenny Ly and Ryan Bui
/*****************************************************************************/
 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h" 

using namespace std;
#include "variables.h" 
#include "readfile.h"

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4>& transfstack, float* values)
{
    mat4 transform = transfstack.top();
    vec4 valvec = vec4(values[0], values[1], values[2], values[3]);
    vec4 newval = transform * valvec;
    for (int i = 0; i < 4; i++) values[i] = newval[i];
}

void rightmultiply(const mat4& M, stack<mat4>& transfstack)
{
    mat4& T = transfstack.top();
    T = T * M;
}

// Function to read the input data values
bool readvals(stringstream& s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void readfile(const char* filename)
{
    string str, cmd;
    ifstream in;
    in.open(filename);
    if (in.is_open()) {

        stack <mat4> transfstack;
        transfstack.push(mat4(1.0));  // identity

        getline(in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd;
                int i;
                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "directional") {
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        int addIndex = 4 * numused;
                        int addColorIndex = 3 * numused;

                        for (i = 0; i < 3; i++) {
                            lightposn[i + addIndex] = values[i];
                            lightcolor[i + addColorIndex] = values[i + 3];
                        }
                        lightposn[3 + addIndex] = 0.0f;
                        numused++;
                    }
                }
                else if (cmd == "point") {
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        int addIndex = 4 * numused;
                        int addColorIndex = 3 * numused;
                        for (i = 0; i < 3; i++) {
                            lightposn[i + addIndex] = values[i];
                            lightcolor[i + addColorIndex] = values[i + 3];
                        }
                        lightposn[3 + addIndex] = 1.0f;
                        numused++;
                    }
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            attenuation[i] = values[i];
                        }
                    }
                }

                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i];
                        }
                    }
                }
                else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i];
                        }
                    }
                }
                else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i];
                        }
                    }
                }
                else if (cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i];
                        }
                    }
                }
                else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        shininess = values[0];
                    }
                }
                else if (cmd == "size") {
                    validinput = readvals(s, 2, values);
                    if (validinput) {
                        w = (int)values[0]; h = (int)values[1];
                    }
                }
                else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxdepth = values[0];
                    }
                }
                else if (cmd == "output") {
                    s >> output;
                }
                else if (cmd == "camera") {
                    validinput = readvals(s, 10, values); // 10 values eye cen up fov
                    if (validinput) {

                        eyeinit = glm::vec3(values[0], values[1], values[2]);
                        center = glm::vec3(values[3], values[4], values[5]);
                        upinit = vec3(values[6], values[7], values[8]);
                        fovy = values[9];

                    }
                }

                else if (cmd == "sphere" || cmd == "tri") {
                    if (numobjects == maxobjects) { // No more objects 
                        cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
                    }
                    else {
                        if (cmd == "sphere") {
                            validinput = readvals(s, 4, values);

                            if (validinput) {
                                object* obj = &(objects[numobjects]);

                                obj->type = sphere;
                                
                                // Set the object's light properties
                                for (i = 0; i < 3; i++) {
                                    (obj->ambient)[i] = ambient[i];
                                    (obj->diffuse)[i] = diffuse[i];
                                    (obj->specular)[i] = specular[i];
                                    (obj->emission)[i] = emission[i];
                                }
                                obj->shininess = shininess;

                                for (i = 0; i < 3; i++) {
                                    (obj->center)[i] = values[i];
                                }
                                obj->radius = values[3];

                                // Set the object's transform
                                obj->transform = transfstack.top();
                            }
                        }
                        else if (cmd == "tri") {
                            validinput = readvals(s, 3, values);

                            if (validinput) {
                                object* obj = &(objects[numobjects]);

                                obj->type = tri;

                                // Set the object's light properties
                                for (i = 0; i < 3; i++) {
                                    (obj->ambient)[i] = ambient[i];
                                    (obj->diffuse)[i] = diffuse[i];
                                    (obj->specular)[i] = specular[i];
                                    (obj->emission)[i] = emission[i];
                                }
                                obj->shininess = shininess;

                                for (i = 0; i < 3; i++) {
                                    (obj->vertices)[i] = values[i];
                                }

                                // Set the object's transform
                                obj->transform = transfstack.top();
                            }

                        }
                        ++numobjects;
                    }
                }
                else if (cmd == "maxverts") {
                    validinput = readvals(s, 1, values);
                }
                else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            vertexes[numvertexes][i] = values[i];
                        }
                    }
                    numvertexes++;
                }

                else if (cmd == "translate") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {

                        rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);

                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {

                        rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);

                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {

                        rightmultiply(glm::mat4(Transform::rotate(values[3], glm::vec3(values[0], values[1], values[2]))), transfstack);

                    }
                }

                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                }
                else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    }
                    else {
                        transfstack.pop();
                    }
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline(in, str);
        }

    }
    else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}
