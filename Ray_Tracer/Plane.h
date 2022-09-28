#pragma once
#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include "Ray.h"
#include "Shape.h"
/*
* The Plane class extends the Shape class. The intersect() function is overrriden
* for the specificities of a plane intersection calculation. The Plane shape holds
* data on its vertices and edges. 
*/
class Plane :public Shape {
public:
    glm::vec3 pointA;
    glm::vec3 pointB;
    glm::vec3 pointC;
    glm::vec3 AB;
    glm::vec3 BC;
    glm::vec3 CA;

    Plane(std::string name, glm::vec3 PointA, glm::vec3 PointB, glm::vec3 PointC, glm::vec3 color) {
        this->name = name;
        this->pointA = PointA;
        this->pointB = PointB;
        this->pointC = PointC;
        AB = PointB - PointA;
        BC = PointC - PointB;
        CA = PointA - PointC;
        this->color = color;
        this->mirror = true;
        this->vertexNormal = glm::normalize(glm::cross(AB, BC));
    }

    bool intersect(Ray& ray) {
        //Checks if the plane is not the one that the Ray is on
        if (dot(ray.direction, vertexNormal) != 0) {
            float intersectDist = dot((pointA - ray.origin), vertexNormal) / dot(ray.direction, vertexNormal);
            // Checks if intersection point is on the plane
            if (round(dot((ray.origin + (ray.direction * intersectDist) - pointA), vertexNormal)) == 0) {
                glm::vec3 point = ray.origin + (ray.direction * intersectDist);
                // Checks if intersection point is specifically on the Plane object
                if (dot(glm::cross(AB, point - pointA), vertexNormal) > 0 && dot(glm::cross(BC, point - pointB), vertexNormal) > 0 && dot(glm::cross(CA, point - pointC), vertexNormal) > 0) {
                    intersectedPoint = ray.origin + (ray.direction * intersectDist);
                    intersectDistance = intersectDist;
                    return true;
                }
            }
        }
        return false;
    }
};