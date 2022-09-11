// Based on templates from learnopengl.com
#define GLFW_INCLUDE_NONE
#define GLEW_STATIC
#include "GLFW/glfw3.h"
#include "GL/glew.h"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"

#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;

class LightSource {
public:
    glm::vec3 center;
    float intensity;
    glm::vec3 LookAt;

    LightSource(glm::vec3 center, float intensity, glm::vec3 LookAt){
        this->center = center;
        this->intensity = intensity;
        this->LookAt = LookAt;
    }
};

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3& origin, glm::vec3& direction){
        this->origin = origin;
        this->direction = glm::normalize(direction);
    }
};

class Camera {
    public:
        glm::vec3 viewP;
        float distance;
        glm::vec3 up;
        glm::vec3 LookAt;
        bool isOrtho;
        int screenSize[4] = { 256, -256, 256, -256 };

        Camera(glm::vec3 viewOrigin, float distance, glm::vec3 up, glm::vec3 LookAt, bool isOrtho) {
            this->viewP = viewOrigin;
            this->distance = distance;
            this->up = up;
            this->LookAt = LookAt;
            this->isOrtho = isOrtho;
        }

        Ray getRay(float x, float y, int WIDTH, int HEIGHT) {
            glm::vec3 w = glm::normalize(glm::vec3(0, 0, 0) - LookAt);    
            glm::vec3 u = glm::normalize(glm::cross(w, up));
            glm::vec3 v = glm::normalize(glm::cross(w, u));
            if (isOrtho){
                this->screenSize[1] = 0;
                this->screenSize[3] = 0;
            }
            float screen1 = screenSize[1] - screenSize[0];
            float screen2 = screenSize[3] - screenSize[2];
            glm::vec3 direction;
            glm::vec3 b;
            if (isOrtho){
                direction = LookAt;
                b = viewP + ((screenSize[1] - ((screen1 * (x + 0.5f)) / WIDTH)) * u) + ((screenSize[3] - ((screen2 * (y + 0.5f)) / HEIGHT)) * v);
            }
            else{
                b = viewP;
                direction = (-distance) * w + ((screenSize[1] - ((screen1 * (x + 0.5f)) / WIDTH)) * u) + ((screenSize[3] - ((screen2 * (y + 0.5f)) / HEIGHT)) * v);
            }
            Ray r(b, direction);
            return r;
        }
};

class Shape {
public:
    std::string name;
    glm::vec3 vertNormal;
    glm::vec3 color;
    glm::vec3 currPoint;
    float currT;
    Shape() {
        this->name = "";
        this->currT = 0;
        this->vertNormal = glm::vec3(0, 0, 0);
        this->color = glm::vec3(0, 0, 0);
        this->currPoint = glm::vec3(0, 0, 0);
    };
    virtual bool intersect(Ray& ray) {
        return true;
    };
};

class Sphere :public Shape {
public:
    glm::vec3 center;
    float radius;

    Sphere(std::string name, glm::vec3 center, float radius, glm::vec3 color){
        this->name = name;
        this->center = center;
        this->radius = radius;
        this->color = color;
        vertNormal = glm::vec3(0, 0, 0);
    }

    bool intersect(Ray& ray) {
        glm::vec3 L = center - ray.origin;
        float Tca = dot(L, ray.direction);
        if (Tca <= 0)
            return false;
        float d = sqrt(dot(L, L) - pow(Tca, 2));
        if (d < 0 || d > radius)
            return false;
        float Thc = sqrt(pow(radius,2) - pow(d,2));

        float t0 = Tca - Thc;
        float t1 = Tca + Thc;

        if (t0 < 0 || t1 < 0)
            return false;
        float tmin;

        if (t0 < t1)
            tmin = t0;
        else
            tmin = t1;

        glm::vec3 point = ray.origin + (ray.direction * tmin);
        currPoint = point;
        currT = tmin;
        vertNormal = glm::normalize(point - center);
        if (round(dot(point - center, point - center)) - pow(radius,2) == 0)
            return true;
        else
            return false;

    }
};

class Plane :public Shape {
public:
    glm::vec3 pointA;
    glm::vec3 pointB;
    glm::vec3 pointC;
    glm::vec3 AB;
    glm::vec3 BC;
    glm::vec3 CA;

    Plane(std::string name, glm::vec3 PointA, glm::vec3 PointB, glm::vec3 PointC, glm::vec3 color){
        this->name = name;
        this->pointA = PointA;
        this->pointB = PointB;
        this->pointC = PointC;
        AB = PointB - PointA;
        BC = PointC - PointB;
        CA = PointA - PointC;
        this->color = color;
        this->vertNormal = glm::normalize(glm::cross(AB, BC));
    }

    bool intersect(Ray& ray){
        if (dot(ray.direction, vertNormal) != 0){
            float t1 = dot((pointA - ray.origin), vertNormal) / dot(ray.direction, vertNormal);
            if (round(dot((ray.origin + (ray.direction * t1) - pointA), vertNormal)) == 0) {// is on plane
                glm::vec3 point = ray.origin + (ray.direction * t1);
                if (dot(glm::cross(AB, point - pointA), vertNormal) > 0 && dot(glm::cross(BC, point - pointB), vertNormal) > 0 && dot(glm::cross(CA, point - pointC), vertNormal) > 0){
                    currPoint = ray.origin + (ray.direction * t1);
                    currT = t1;
                    return true;
                }
            }
        }
        return false;
    }

};

class Tetrahedron :public Shape {
public:
    glm::vec3 pointA;
    glm::vec3 pointB;
    glm::vec3 pointC;
    glm::vec3 pointD;
    glm::vec3 AB;
    glm::vec3 BC;
    glm::vec3 CD;
    glm::vec3 CA;
    glm::vec3 DA;
    glm::vec3 BD;
    glm::vec3 DB;
    glm::vec3 AD;
    glm::vec3 DC;

    glm::vec3 n1;
    glm::vec3 n2;
    glm::vec3 n3;
    glm::vec3 n4;

    Tetrahedron(std::string name, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD, glm::vec3 color){
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

    bool intersect(Ray& ray){
        float t1=-1, t2=-1 , t3=-1, t4=-1;
        bool face[]{ false, false, false, false };
        if (dot(ray.direction, n1) != 0){
            t1 = dot((pointA - ray.origin), n1) / dot(ray.direction, n1);
            if (round(dot((ray.origin + (ray.direction * t1) - pointA), n1)) == 0){ // is on plane
                glm::vec3 point = ray.origin + (ray.direction * t1);
                if (t1 >= 0 && dot(glm::cross(AB, point - pointA), n1) > 0 && dot(glm::cross(BC, point - pointB), n1) > 0 && dot(glm::cross(CA, point - pointC), n1) > 0)  //tri 1
                    face[0] = true;
            }
        }
        if (dot(ray.direction, n2) != 0){
            t2 = dot((pointD - ray.origin), n2) / dot(ray.direction, n2);
            if (round(dot((ray.origin + (ray.direction * t2) - pointD), n2)) == 0) { 
                glm::vec3 point = ray.origin + (ray.direction * t2);
                if (t2 >= 0 && dot(glm::cross(AB, point - pointA), n2) > 0 && dot(glm::cross(BD, point - pointB), n2) > 0 && dot(glm::cross(DA, point - pointD), n2) > 0)  //tri 2
                    face[1]= true;
            }
        }
        if (dot(ray.direction, n3) != 0){
            t3 = dot((pointB - ray.origin), n3) / dot(ray.direction, n3);
            if (round(dot((ray.origin + (ray.direction * t3) - pointB), n3)) == 0){
                glm::vec3 point = ray.origin + (ray.direction * t3);
                if (t3 >= 0 && dot(glm::cross(BC, point - pointB), n3) > 0 && dot(glm::cross(CD, point - pointC), n3) > 0 && dot(glm::cross(DB, point - pointD), n3) > 0)  //tri 3
                    face[2] = true;
            }
        }
        if (dot(ray.direction, n4) != 0){
            t4 = dot((pointC - ray.origin), n4) / dot(ray.direction, n4);
            if (round(dot((ray.origin + (ray.direction * t4) - pointC), n4)) == 0){
                glm::vec3 point = ray.origin + (ray.direction * t4);
                if (t4 >= 0 && dot(glm::cross(CA, point - pointC), n4) > 0 && dot(glm::cross(AD, point - pointA), n4) > 0 && dot(glm::cross(DC, point - pointD), n4) > 0)  //tri 4
                    face[3] = true;
            }
        }

        float ts[]{ t1,t2,t3,t4};
        glm::vec3 ns[]{ n1,n2,n3,n4 };
        float tmin = std::numeric_limits<float>::max();
        int minIndex = 0;
        if (face[0] == false && face[1] == false && face[2] == false && face[3] == false)
            return false;
        else{
            for (int i = 0; i < 4; i++){
                if (face[i] && ts[i] < tmin){
                    tmin = ts[i];
                    minIndex = i;
                }
            }
            vertNormal = glm::normalize(ns[minIndex]);
            currPoint = ray.origin + (ray.direction * tmin);
            currT = tmin;
            return true;
        }
    }
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture1, TexCoord);\n"
"}\n\0";

glm::vec3 trace(Shape* shape, LightSource* light, Ray& ray, std::vector<Shape*> shapes, bool notMirror){
    float Lsx, Lsy, Lsz;
    glm::vec3 Vl = glm::vec3(0, 0, 0) - light->LookAt;
    glm::vec3 LVec;
    glm::vec3 Ve = glm::normalize(ray.origin - shape->currPoint);
    glm::vec3 Vh = glm::normalize((Vl + Ve) / abs(Vl + Ve));

    float Ldx = std::min(255.0f, shape->color.x * light->intensity * std::max(0.0f, dot(shape->vertNormal, Vl)));
    float Lax = std::min(255.0f, shape->color.x*light->intensity);
    Lsx = std::min(255.0f, light->intensity *250* pow(std::max(0.0f, dot(shape->vertNormal, Vh)), 100));

    float Ldy = std::min(255.0f, shape->color.y * light->intensity * std::max(0.0f, dot(shape->vertNormal, Vl)));
    float Lay = std::min(255.0f, shape->color.y* light->intensity);
    Lsy = std::min(255.0f, light->intensity * 250 * pow(std::max(0.0f, dot(shape->vertNormal, Vh)),100));

    float Ldz = std::min(255.0f, shape->color.z * light->intensity * std::max(0.0f, dot(shape->vertNormal, Vl)));
    float Laz = std::min(255.0f, shape->color.z* light->intensity);
    Lsz = std::min(255.0f, light->intensity * 250 * pow(std::max(0.0f, dot(shape->vertNormal, Vh)), 100));

    for (unsigned int i = 0; i < shapes.size(); i++){
        Ray r(shape->currPoint, Vl);
        if (shapes[i]->name.compare(shape->name) != 0 && shapes[i]->intersect(r) && shapes[i]->name.compare("plane") != 0 && !glm::all(glm::equal(Vl, light->LookAt))){
            Ldx *= 0.2f;
            Ldy *= 0.2f;
            Ldz *= 0.2f;
            break;
        }
    }

    if (notMirror){
        glm::vec3 refl = (glm::vec3(0, 0, 0) - Ve) - ((2 * dot(glm::vec3(0, 0, 0) - Ve, shape->vertNormal)) * shape->vertNormal);
        Ray reflRay(shape->currPoint, refl);
        if (!shapes[0]->intersect(reflRay) && !shapes[1]->intersect(reflRay) && !shapes[2]->intersect(reflRay)) {}
        else{
            bool intersections[]{ false, false, false };
            intersections[0] = shapes[0]->intersect(reflRay);
            intersections[1] = shapes[1]->intersect(reflRay);
            intersections[2] = shapes[2]->intersect(reflRay);
            float smallestT = 1000;
            int Ind = 0;
            for (unsigned int h = 0; h < shapes.size() - 1; h++) {
                if (intersections[h] && shapes[h]->currT < smallestT) {
                    smallestT = shapes[h]->currT;
                    Ind = h;
                }
            }
            Lsx = 0.4f * (trace(shapes[Ind], light, reflRay, shapes, false)).x;
            Lsy = 0.4f * (trace(shapes[Ind], light, reflRay, shapes, false)).y;
            Lsz = 0.4f * (trace(shapes[Ind], light, reflRay, shapes, false)).z;
        }
    }
    if (Ldx +Lax +Lsx <= 255)
        LVec.x = Ldx + Lax + Lsx;
    else
        LVec.x = 255;
    if (Ldy + Lay + Lsy <= 255)
        LVec.y = Ldy + Lay +Lsy;
    else
        LVec.y = 255;
    if (Ldz + Laz +Lsz <= 255)
        LVec.z = Ldz + Laz +Lsz;
    else
        LVec.z = 255;

    return LVec;
}

Camera* camera = new Camera(glm::vec3(0, -120, 0), 0, glm::vec3(0, 1, 0), glm::vec3(0, 0.4f, 1), true);

#define WIDTH 512
#define HEIGHT 512
void RayTrace(){
    std::vector<Shape*> shapes;

    LightSource* light = new LightSource(glm::vec3(0, 1, 0), 0.4f, glm::vec3(1.1f, 0.8f, 1.2f));

    Sphere* sphere_1 = new Sphere("sphere1", glm::vec3(40, -130, 220), 30, (glm::vec3(15, 250, 60)));
    Sphere* sphere_2 = new Sphere("sphere2", glm::vec3(160, -140, 350), 40, glm::vec3(240, 15, 50));

    Plane* plane = new Plane("plane", glm::vec3(-128, -100, 500), glm::vec3(128, -100, -500), glm::vec3(384, -100, 500), glm::vec3(140, 130, 120));

    Tetrahedron* tetra = new Tetrahedron("tetra", glm::vec3(135, -100, 251), glm::vec3(105, -100, 261), glm::vec3(145, -100, 266), glm::vec3(125, -140, 264), glm::vec3(15, 60, 230));
    shapes.push_back(sphere_1);
    shapes.push_back(sphere_2);
    shapes.push_back(tetra);
    shapes.push_back(plane);
    unsigned char pixels[WIDTH * HEIGHT * 3];

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            Ray ray = camera->getRay((float)j, (float)i, WIDTH, HEIGHT);
            if (!shapes[0]->intersect(ray) && !shapes[1]->intersect(ray) && !shapes[2]->intersect(ray) && !shapes[3]->intersect(ray)) {
                pixels[(i * WIDTH + j) * 3] = (char)40;
                pixels[(i * WIDTH + j) * 3 + 1] = (char)40;
                pixels[(i * WIDTH + j) * 3 + 2] = (char)40;
            }
            else {
                bool intersections[]{ false, false, false, false };
                intersections[0] = shapes[0]->intersect(ray);
                intersections[1] = shapes[1]->intersect(ray);
                intersections[2] = shapes[2]->intersect(ray);
                intersections[3] = shapes[3]->intersect(ray);
                float smallestT = 1000;
                int Ind = 0;
                for (unsigned int h = 0; h < shapes.size(); h++) {
                    if (intersections[h] && shapes[h]->currT < smallestT) {
                        smallestT = shapes[h]->currT;
                        Ind = h;
                    }
                }
                bool mirror = false;
                if (Ind == 3)     
                    mirror = true;
                glm::vec3 Ld = trace(shapes[Ind], light, ray, shapes, mirror);
                pixels[(i * WIDTH + j) * 3] = (char)Ld.x;
                pixels[(i * WIDTH + j) * 3 + 1] = (char)Ld.y;
                pixels[(i * WIDTH + j) * 3 + 2] = (char)Ld.z;
            }
        }
    }

    unsigned char* data = &pixels[0];
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
}

int main(){
    std::cout << "Hello! Welcome to my Ray Tracer." << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << "To change camera from Orthographic to Perspective or vice versa, press Z while in the Display Window." << std::endl;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ray Tracer", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    RayTrace();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)){
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //const char* file = "C:\OpenGLFiles";
    //saveImage(file, window);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        std::cout << "\nGenerating image with";
        if (camera->isOrtho){
            camera = new Camera(glm::vec3(0, -180, -50), 590, glm::vec3(0, 1, 0), glm::vec3(0.4f, 0.1f, 1), false);
            std::cout << " Perspective camera..." << std::endl;
        }
        else{
            camera->distance = 0;
            camera->viewP = glm::vec3(0, -120, 0);
            camera->LookAt = glm::vec3(0, 0.4f, 1);
            camera->isOrtho = true;
            std::cout << " Orthographic camera..." << std::endl;
        }
        RayTrace();
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}