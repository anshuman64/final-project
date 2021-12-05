/**************************************************
Camera is a class for a camera object.
*****************************************************/
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera {
public:
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;
    float fovy;
    float aspect;
    glm::mat4 camera_matrix  = glm::mat4(1.0f);   // camera matrix


    Camera(glm::vec3 _eye, glm::vec3 _target, glm::vec3 _up, float _fovy, float width, float height) :
            eye(_eye), target(_target), up(_up), fovy(_fovy) {
        // Calculate camera matrix
        glm::vec3 targetToEye = eye - target;

        glm::vec3 Cez = glm::normalize(targetToEye);
        glm::vec3 Cey = glm::normalize(up - glm::dot(Cez,up)*Cez);
        glm::vec3 Cex = glm::cross(Cey,Cez);

        camera_matrix[0] = glm::vec4(Cex,0.0f);
        camera_matrix[1] = glm::vec4(Cey,0.0f);
        camera_matrix[2] = glm::vec4(Cez,0.0f);
        camera_matrix[3] = glm::vec4(eye,1.0f);

        aspect = width / height;
    };
};

#endif
