#include "Camera.h"

Camera::Camera(glm::vec3 viewOrigin, float distance, glm::vec3 up, glm::vec3 lookAt, bool isOrtho) {
    this->viewPlane = viewOrigin;
    this->distance = distance;
    this->up = up;
    this->lookAt = lookAt;
    this->isOrtho = isOrtho;
}

//getRay() generates the ray out of a given pixel (if using orthographic) or out of the center of the camera (if using perspective)
Ray Camera::getRay(float x, float y, int WIDTH, int HEIGHT) {
    glm::vec3 w = glm::normalize(glm::vec3(0, 0, 0) - lookAt);
    glm::vec3 u = glm::normalize(glm::cross(w, up));
    glm::vec3 v = glm::normalize(glm::cross(w, u));

    //Centers the camera screen at (0,0) rather than (-256, -256) for perspective camera
    if (isOrtho) {
        this->screenSize[1] = 0;
        this->screenSize[3] = 0;
    }
    float screenWidth = screenSize[1] - screenSize[0];
    float screenHeight = screenSize[3] - screenSize[2];

    glm::vec3 direction;
    glm::vec3 origin;
    if (isOrtho) {
        //Generates a ray orthographic to the camera's viewPlane 
        direction = lookAt;
        origin = viewPlane + ((screenSize[1] - ((screenWidth * (x + 0.5f)) / WIDTH)) * u) + ((screenSize[3] - ((screenHeight * (y + 0.5f)) / HEIGHT)) * v);
    }
    else {
        //Generates a ray at the appropriate angle out of the camera's perspective view
        origin = viewPlane;
        direction = (-distance) * w + ((screenSize[1] - ((screenWidth * (x + 0.5f)) / WIDTH)) * u) + ((screenSize[3] - ((screenHeight * (y + 0.5f)) / HEIGHT)) * v);
    }
    Ray r(origin, direction);
    return r;
}

Camera::~Camera() {}

void Camera::setDistance(float distance) {
    this->distance = distance;
}

void Camera::setIsOrtho(bool isOrtho) {
    this->isOrtho = isOrtho;
}

void Camera::setLookAt(glm::vec3 lookAt){
    this->lookAt = lookAt;
}

void Camera::setViewPlane(glm::vec3 viewPlane) {
    this->viewPlane = viewPlane;
}
