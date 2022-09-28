#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
/*
* The LightSource class holds data regarding the camera screenspace from where
* the rays are projected out to intersect with the scene objects.
* A Camera object can either be Orthographic or Perspective. The getRay()
* function generates the per-pixel rays.
*/
class LightSource {
public:
    glm::vec3 center;
    float intensity;
    glm::vec3 lookAt;

    LightSource(glm::vec3 center, float intensity, glm::vec3 lookAt) {
        this->center = center;
        this->intensity = intensity;
        this->lookAt = lookAt;
    }

    ~LightSource() {}
};

