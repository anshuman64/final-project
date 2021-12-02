/**************************************************
Camera is a class for a camera object.
*****************************************************/
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera {
public:
    glm::vec3 eye;// position of the eye
    glm::vec3 target;  // look at target
    glm::vec3 up;      // up vector
    float fovy;  // field of view in degrees
    float aspect; // aspect ratio
    float near = 0.01f; // near clipping distance
    float far = 100.0f; // far clipping distance

    glm::mat4 cam  = glm::mat4(1.0f);   // camera matrix
    glm::mat4 view = glm::mat4(1.0f);   // view matrix
    glm::mat4 proj = glm::mat4(1.0f);   // projection matrix

    Camera(glm::vec3 _eye,
            glm::vec3 _target,
            glm::vec3 _up,
            float _fovy,
            float width,
            float height) :
            eye(_eye),
            target(_target),
            up(_up),
            fovy(_fovy) {
        // View Matrix
        glm::vec3 targetToEye = eye - target;

        glm::vec3 Cez = glm::normalize(targetToEye);
        glm::vec3 Cey = glm::normalize(up - glm::dot(Cez,up)*Cez);
        glm::vec3 Cex = glm::cross(Cey,Cez);

        cam[0] = glm::vec4(Cex,0.0f);
        cam[1] = glm::vec4(Cey,0.0f);
        cam[2] = glm::vec4(Cez,0.0f);
        cam[3] = glm::vec4(eye,1.0f);

        view = glm::inverse(cam);

        // Projection Matrix
        aspect = width / height;
        
        float fovy_rad = fovy * M_PI/180.0f;
        float tan = glm::tan(fovy_rad/2.0f);
        proj = glm::mat4(1.0f/(aspect*tan), 0.0f,     0.0f, 0.0f,
                         0.0f,              1.0f/tan, 0.0f, 0.0f,
                         0.0f, 0.0f,    -(far+near)/(far-near), -1.0f,
                         0.0f, 0.0f, -2.0f*far*near/(far-near), 0.0f);

    };
};

#endif
