#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0, 0.0, 5.0); // Initial eye position, also for resets
vec3 upinit(0.0, 1.0, 0.0); // Initial up position, also for resets
vec3 center(0.0, 0.0, 0.0); // Center look at point 
int amountinit = 5;
int w = 500, h = 500; // width and height 
float fovy = 90.0; // For field of view
#else 
EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 center;
EXTERN int amountinit;
EXTERN int w, h;
EXTERN float fovy;
#endif 


EXTERN mat4 projection, modelview; // The mvp matrices
EXTERN unsigned int projectionPos, modelviewPos; // Uniform locations of the above matrices
enum shape { sphere, tri };

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 100;
EXTERN float lightposn[4 * numLights]; // Light Positions
EXTERN float lightcolor[3 * numLights]; // Light Colors
//EXTERN float lightransf[4 * numLights]; // Lights transformed by modelview
EXTERN int numused;

EXTERN float attenuation[3];
EXTERN int maxdepth;
EXTERN std::string output;

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN vec3 ambient;
EXTERN vec3 diffuse;
EXTERN vec3 specular;
EXTERN vec3 emission;
EXTERN float shininess;

// For multiple objects, read from a file.  
const int maxobjects = 1000000;
EXTERN int numobjects;
EXTERN struct object {
	shape type;
	float size;
	float radius;
	vec3 center;
	int vertices[3];
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;
	mat4 transform;
};
EXTERN object objects[maxobjects];

const int maxverts = 100000;
EXTERN int numvertexes;
EXTERN vec3 vertexes[maxverts];

