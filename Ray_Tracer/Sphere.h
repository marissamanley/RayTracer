#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include "Ray.h"
#include "Shape.h"
/*
* The Sphere class extends the Shape class. The intersect() function is overrriden
* for the specificities of a sphere intersection calculation. The Sphere shape holds 
* data on its center location and radius. 
*/
class Sphere :public Shape {
public:
    glm::vec3 center;
    float radius;

    Sphere(std::string name, glm::vec3 center, float radius, glm::vec3 color) {
        this->name = name;
        this->center = center;
        this->radius = radius;
        this->color = color;
        vertexNormal = glm::vec3(0, 0, 0);
    }

    bool intersect(Ray& ray) {
        glm::vec3 rayToShapeCenter = center - ray.origin;
        float distToShapeCenter = dot(rayToShapeCenter, ray.direction);
        //Check if shape center is behind/inside the camera
        if (distToShapeCenter <= 0) {
            return false;
        }

        float d = sqrt(dot(rayToShapeCenter, rayToShapeCenter) - pow(distToShapeCenter, 2));
        if (d < 0 || d > radius) {
            return false;
        }
        float outerShapeIntersectDist = sqrt(pow(radius, 2) - pow(d, 2));

        //Generates the distance from the Ray origin to both intersection points of the shape
        float intersectDistance1 = distToShapeCenter - outerShapeIntersectDist;
        float intersectDistance2 = distToShapeCenter + outerShapeIntersectDist;

        //Check if shape is partially behind camera
        if (intersectDistance1 < 0 || intersectDistance2 < 0) {
            return false;
        }

        float distCloserToRay;
        //Finds intersection point closest to camera, aka the intersection point visible to the camera
        if (intersectDistance1 < intersectDistance2) {
            distCloserToRay = intersectDistance1;
        }
        else {
            distCloserToRay = intersectDistance2;
        }

        glm::vec3 point = ray.origin + (ray.direction * distCloserToRay);
        intersectedPoint = point;
        intersectDistance = distCloserToRay;
        vertexNormal = glm::normalize(point - center);
        if (round(dot(point - center, point - center)) - pow(radius, 2) == 0) {
            return true;
        }
        else {
            return false;
        }
    }
};