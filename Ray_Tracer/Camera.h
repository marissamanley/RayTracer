#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
#include "Ray.h"
/*
* The Camera class holds data regarding the camera screenspace from where
* the rays are projected out to intersect with the scene objects. 
* A Camera object can either be Orthographic or Perspective. The getRay()
* function generates the per-pixel rays.
*/
class Camera {
public:
    glm::vec3 viewPlane;
    float distance;
    glm::vec3 up;
    glm::vec3 lookAt;
    bool isOrtho;
    int screenSize[4] = { 256, -256, 256, -256 }; //output screen pixel resolution

    Camera(glm::vec3 viewOrigin, float distance, glm::vec3 up, glm::vec3 lookAt, bool isOrtho);

    Ray getRay(float x, float y, int WIDTH, int HEIGHT);
    ~Camera();
    void setViewPlane(glm::vec3 viewPlane);
    void setLookAt(glm::vec3 lookAt);
    void setDistance(float distance);
    void setIsOrtho(bool isOrtho);
};