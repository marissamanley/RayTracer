#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include "Ray.h"
/*
* The Shape class is the parent class for the Sphere, Tetrahedron, and Plane classes.
* It holds common shape elements such as the name and color.
*/
class Shape {
public:
    std::string name;
    bool mirror;
    glm::vec3 vertexNormal;
    glm::vec3 color;
    glm::vec3 intersectedPoint;
    float intersectDistance;
    Shape() {
        this->name = "";
        this->intersectDistance = 0;
        this->vertexNormal = glm::vec3(0, 0, 0);
        this->color = glm::vec3(0, 0, 0);
        this->intersectedPoint = glm::vec3(0, 0, 0);
        this->mirror = false;
    };
    //intersect() function to be overridden checks to see if a given ray intersects with the current Shape object
    virtual bool intersect(Ray& ray) {
        return true;
    };

    ~Shape() {}
};