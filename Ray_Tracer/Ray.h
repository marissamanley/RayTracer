#pragma once
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"
/*
* The Ray class is essentially a custom Vec3 object used to
* represent the rays casted out from the camera screenspace. 
*/
class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3& origin, glm::vec3& direction) {
        this->origin = origin;
        this->direction = glm::normalize(direction);
    }

    ~Ray() {}
};