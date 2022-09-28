#pragma once
#include "RayTracer.h"

RayTracer::RayTracer(){
    light = new LightSource(glm::vec3(0, 1, 0), 0.4f, glm::vec3(1.1f, 0.8f, 1.2f));
    camera = new Camera(glm::vec3(0, -120, 0), 0, glm::vec3(0, 1, 0), glm::vec3(0, 0.4f, 1), true);
    Sphere* sphere_1 = new Sphere("sphere1", glm::vec3(40, -130, 220), 30, (glm::vec3(15, 250, 60)));
    Sphere* sphere_2 = new Sphere("sphere2", glm::vec3(160, -140, 350), 40, glm::vec3(240, 15, 50));
    Plane* plane = new Plane("plane", glm::vec3(-128, -100, 500), glm::vec3(128, -100, -500), glm::vec3(384, -100, 500), glm::vec3(140, 130, 120));
    Tetrahedron* tetra = new Tetrahedron("tetra", glm::vec3(135, -100, 251), glm::vec3(105, -100, 261), glm::vec3(145, -100, 266), glm::vec3(125, -140, 264), glm::vec3(15, 60, 230));
    shapes.push_back(sphere_1);
    shapes.push_back(sphere_2);
    shapes.push_back(tetra);
    shapes.push_back(plane);
}

// Using the single ray and single shape as input, this function calculates the
// output pixel color based on the shape color, diffuse/ambient/specular lighting intensities, 
// possible shadow presence, and possible relfective status. 
glm::vec3 RayTracer::Trace(Shape* shape, Ray& ray) {
    // Specular lighting x,y,z variables
    float Lsx, Lsy, Lsz;

    glm::vec3 vLookAt = glm::vec3(0, 0, 0) - light->lookAt;
    glm::vec3 pixelColor;
    glm::vec3 Ve = glm::normalize(ray.origin - shape->intersectedPoint);
    glm::vec3 Vh = glm::normalize((vLookAt + Ve) / abs(vLookAt + Ve));

    // Diffuse, ambient, and specular x values
    float Ldx = std::min(255.0f, shape->color.x * light->intensity * std::max(0.0f, dot(shape->vertexNormal, vLookAt)));
    float Lax = std::min(255.0f, shape->color.x * light->intensity);
    Lsx = std::min(255.0f, light->intensity * 250 * pow(std::max(0.0f, dot(shape->vertexNormal, Vh)), 100.0f));

    // Diffuse, ambient, and specular y values
    float Ldy = std::min(255.0f, shape->color.y * light->intensity * std::max(0.0f, dot(shape->vertexNormal, vLookAt)));
    float Lay = std::min(255.0f, shape->color.y * light->intensity);
    Lsy = std::min(255.0f, light->intensity * 250 * pow(std::max(0.0f, dot(shape->vertexNormal, Vh)), 100.0f));

    // Diffuse, ambient, and specular z values
    float Ldz = std::min(255.0f, shape->color.z * light->intensity * std::max(0.0f, dot(shape->vertexNormal, vLookAt)));
    float Laz = std::min(255.0f, shape->color.z * light->intensity);
    Lsz = std::min(255.0f, light->intensity * 250 * pow(std::max(0.0f, dot(shape->vertexNormal, Vh)), 100.0f));

    for (unsigned int i = 0; i < shapes.size(); i++) {
        Ray r(shape->intersectedPoint, vLookAt);
        // Checks if intersected point is in the shadow of another Shape
        if (shapes[i]->name.compare(shape->name) != 0 && shapes[i]->intersect(r) && shapes[i]->name.compare("plane") != 0 && !glm::all(glm::equal(vLookAt, light->lookAt))) {
            Ldx *= 0.2f;
            Ldy *= 0.2f;
            Ldz *= 0.2f;
            break;
        }
    }

    if (shape->mirror) {
        glm::vec3 refl = (glm::vec3(0, 0, 0) - Ve) - ((2 * dot(glm::vec3(0, 0, 0) - Ve, shape->vertexNormal)) * shape->vertexNormal);
        Ray reflRay(shape->intersectedPoint, refl);
        if (shapes[0]->intersect(reflRay) || shapes[1]->intersect(reflRay) || shapes[2]->intersect(reflRay)) {
            bool intersections[]{ false, false, false };
            intersections[0] = shapes[0]->intersect(reflRay);
            intersections[1] = shapes[1]->intersect(reflRay);
            intersections[2] = shapes[2]->intersect(reflRay);
            float shortestDistance = std::numeric_limits<float>::max();
            int ind = 0;
            for (unsigned int h = 0; h < shapes.size() - 1; h++) {
                if (intersections[h] && shapes[h]->intersectDistance < shortestDistance) {
                    shortestDistance = shapes[h]->intersectDistance;
                    ind = h;
                }
            }
            shape->mirror = false;
            Lsx = 0.4f * (Trace(shapes[ind], reflRay)).x;
            Lsy = 0.4f * (Trace(shapes[ind], reflRay)).y;
            Lsz = 0.4f * (Trace(shapes[ind], reflRay)).z;
            shape->mirror = true;
        }
    }
    if (Ldx + Lax + Lsx <= 255) {
        pixelColor.x = Ldx + Lax + Lsx;
    }
    else {
        pixelColor.x = 255;
    }
    if (Ldy + Lay + Lsy <= 255) {
        pixelColor.y = Ldy + Lay + Lsy;
    }
    else {
        pixelColor.y = 255;
    }
    if (Ldz + Laz + Lsz <= 255) {
        pixelColor.z = Ldz + Laz + Lsz;
    }
    else {
        pixelColor.z = 255;
    }
    return pixelColor;
}

#define WIDTH 512
#define HEIGHT 512
// This function creates the display output by generating pixel rays and calling the shapes'
// Intersect() functions to determine if the ray intersects. In the case of intersection,
// the Trace() function is called for a given ray and given intersected shape to determine pixel color. 
// If no shape is intersected, the pixel color is set to the default background color.
void RayTracer::RayTrace() {
    unsigned char pixels[WIDTH * HEIGHT * 3];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            // Ray is generated per pixel
            Ray ray = camera->getRay((float)j, (float)i, WIDTH, HEIGHT);

            // Sets pixel to background color of grey if no Shapes are intersected
            if (!shapes[0]->intersect(ray) && !shapes[1]->intersect(ray) && !shapes[2]->intersect(ray) && !shapes[3]->intersect(ray)) {
                pixels[(i * WIDTH + j) * 3] = (char)40;
                pixels[(i * WIDTH + j) * 3 + 1] = (char)40;
                pixels[(i * WIDTH + j) * 3 + 2] = (char)40;
            }
            else { // Discerns intersected shape closest to camera
                bool intersections[]{ false, false, false, false };
                intersections[0] = shapes[0]->intersect(ray);
                intersections[1] = shapes[1]->intersect(ray);
                intersections[2] = shapes[2]->intersect(ray);
                intersections[3] = shapes[3]->intersect(ray);
                float closestIntersection = std::numeric_limits<float>::max();
                int ind = 0;
                for (unsigned int h = 0; h < shapes.size(); h++) {
                    if (intersections[h] && shapes[h]->intersectDistance < closestIntersection) {
                        closestIntersection = shapes[h]->intersectDistance;
                        ind = h;
                    }
                }
                glm::vec3 pixelColor = Trace(shapes[ind], ray);
                pixels[(i * WIDTH + j) * 3] = (char)pixelColor.x;
                pixels[(i * WIDTH + j) * 3 + 1] = (char)pixelColor.y;
                pixels[(i * WIDTH + j) * 3 + 2] = (char)pixelColor.z;
            }
        }
    }

    unsigned char* data = &pixels[0];
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
}


Camera* RayTracer::getCamera() {
    return camera;
}

RayTracer::~RayTracer(){
    delete light;
    for (int j = shapes.size(); j < 0; j++) {
        Shape* s = shapes[j];
        shapes.pop_back();
        delete s;
    }
}

void RayTracer::setCamera(Camera* c) {
    delete this->camera;
    this->camera = c;
}