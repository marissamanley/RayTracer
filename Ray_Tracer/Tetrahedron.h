#pragma once
#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include "Ray.h"
#include "Shape.h"
/*
* The Tetrahedron class extends the Shape class. The intersect() function is overrriden
* for the specificities of a tetrahedron intersection calculation. The Tetrahedron shape holds
* data on its vertices and edges.
*/
class Tetrahedron :public Shape {
public:
    glm::vec3 pointA, pointB, pointC, pointD;
    //Tetrahedrom edges
    glm::vec3 AB, BC, CD, CA, DA, BD, DB, AD, DC;
    //Tetrahedron face normals
    glm::vec3 n1, n2, n3, n4;

    Tetrahedron(std::string name, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD, glm::vec3 color) {
        this->color = color;
        this->name = name;

        this->pointA = pointA;
        this->pointB = pointB;
        this->pointC = pointC;
        this->pointD = pointD;

        AB = pointA - pointB;
        BC = pointB - pointC;
        CA = pointC - pointA;

        //AB
        BD = pointB - pointD;
        DA = pointD - pointA;

        //BC
        CD = pointC - pointD;
        DB = pointD - pointB;

        //CA
        AD = pointA - pointD;
        DC = pointD - pointC;

        n1 = glm::normalize(glm::cross(BC, AB));
        n2 = glm::normalize(glm::cross(BD, AB));
        n3 = glm::normalize(glm::cross(CD, BC));
        n4 = glm::normalize(glm::cross(AD, CA));
    }

    bool intersect(Ray& ray) {
        float intersectDist1 = -1, intersectDist2 = -1, intersectDist3 = -1, intersectDist4 = -1;
        bool faceIntersected[]{ false, false, false, false };

        //Face ABC
        if (dot(ray.direction, n1) != 0) {
            intersectDist1 = dot((pointA - ray.origin), n1) / dot(ray.direction, n1);
            // checks if intersection is on face ABC's plane
            if (round(dot((ray.origin + (ray.direction * intersectDist1) - pointA), n1)) == 0) {
                glm::vec3 point = ray.origin + (ray.direction * intersectDist1);
                // checks if intersection is through face ABC
                if (intersectDist1 >= 0 && dot(glm::cross(AB, point - pointA), n1) > 0 && dot(glm::cross(BC, point - pointB), n1) > 0 && dot(glm::cross(CA, point - pointC), n1) > 0) {
                    faceIntersected[0] = true;
                }
            }
        }
        //Face ABD
        if (dot(ray.direction, n2) != 0) {
            intersectDist2 = dot((pointD - ray.origin), n2) / dot(ray.direction, n2);
            if (round(dot((ray.origin + (ray.direction * intersectDist2) - pointD), n2)) == 0) {
                glm::vec3 point = ray.origin + (ray.direction * intersectDist2);
                if (intersectDist2 >= 0 && dot(glm::cross(AB, point - pointA), n2) > 0 && dot(glm::cross(BD, point - pointB), n2) > 0 && dot(glm::cross(DA, point - pointD), n2) > 0) {
                    faceIntersected[1] = true;
                }
            }
        }
        //Face BCD
        if (dot(ray.direction, n3) != 0) {
            intersectDist3 = dot((pointB - ray.origin), n3) / dot(ray.direction, n3);
            if (round(dot((ray.origin + (ray.direction * intersectDist3) - pointB), n3)) == 0) {
                glm::vec3 point = ray.origin + (ray.direction * intersectDist3);
                if (intersectDist3 >= 0 && dot(glm::cross(BC, point - pointB), n3) > 0 && dot(glm::cross(CD, point - pointC), n3) > 0 && dot(glm::cross(DB, point - pointD), n3) > 0) {
                    faceIntersected[2] = true;
                }
            }
        }
        //Face ACD
        if (dot(ray.direction, n4) != 0) {
            intersectDist4 = dot((pointC - ray.origin), n4) / dot(ray.direction, n4);
            if (round(dot((ray.origin + (ray.direction * intersectDist4) - pointC), n4)) == 0) {
                glm::vec3 point = ray.origin + (ray.direction * intersectDist4);
                if (intersectDist4 >= 0 && dot(glm::cross(CA, point - pointC), n4) > 0 && dot(glm::cross(AD, point - pointA), n4) > 0 && dot(glm::cross(DC, point - pointD), n4) > 0) {
                    faceIntersected[3] = true;
                }
            }
        }

        float dists[]{ intersectDist1,intersectDist2,intersectDist3,intersectDist4 };
        glm::vec3 normals[]{ n1,n2,n3,n4 };
        float shortestDist = std::numeric_limits<float>::max();
        int minIndex = 0;

        //Checks if none of the faces were intersected
        if (faceIntersected[0] == false && faceIntersected[1] == false && faceIntersected[2] == false && faceIntersected[3] == false) {
            return false;
        }
        else {
            //Finds intersected face closest to the Ray
            for (int i = 0; i < 4; i++) {
                if (faceIntersected[i] && dists[i] < shortestDist) {
                    shortestDist = dists[i];
                    minIndex = i;
                }
            }
            vertexNormal = glm::normalize(normals[minIndex]);
            intersectedPoint = ray.origin + (ray.direction * shortestDist);
            intersectDistance = shortestDist;
            return true;
        }
    }
};