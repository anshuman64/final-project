#include "Material.h"
#include "Light.h"

#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

class Ray {
public:
    glm::vec3 point;
    glm::vec3 direction;

    Ray(glm::vec3 _point, glm::vec3 _direction) :
        point(_point), direction(glm::normalize(_direction)) {}
};


class Intersection {
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 normal;
    float     distance = std::numeric_limits<float>::infinity();
    bool      is_hit   = false;
    Material* material;

    void update(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _normal, float _distance, Material* _material);

    bool is_shadow(float light_distance, bool is_directional);
    Ray get_shadow_ray(glm::vec3 light_direction);
    Ray get_mirror_ray();
    Ray get_diffuse_ray();

    glm::vec3 calculate_diffuse(glm::vec3 light_direction);
    glm::vec3 calculate_specular(glm::vec3 light_direction);
};

#endif
