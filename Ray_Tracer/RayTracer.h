#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GL/glew.h"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"

#include "LightSource.h"
#include "Camera.h"
#include "Ray.h"
#include "Shape.h"
#include "Sphere.h"
#include "Tetrahedron.h"
#include "Plane.h"

#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <iostream>
/*
* The purpose of this class is to execute all ray tracing project elements.
* The scene objects, such as the light surce, camera, and shape objects,
* are maintained within the Ray Tracer object. All object-ray intersection calculations
* are completed within the RayTrace() function, and pixel output color is completed
* within the Trace() function.
*/
class RayTracer {
private:
	LightSource* light;
	std::vector<Shape*> shapes;
	Camera* camera;
public:
	RayTracer();
	~RayTracer();
	void RayTrace();
	glm::vec3 Trace(Shape* shape, Ray& ray);
	Camera* getCamera();
	void setCamera(Camera* camera);
};



