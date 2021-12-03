#include "Material.h"

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
    float     distance;
    bool      is_hit = false;
    Material* material;

    Intersection() {};

    void update(Intersection* _intersection) {
        position  = _intersection->position;
        direction = _intersection->direction;
        normal    = _intersection->normal;
        distance  = _intersection->distance;
        is_hit    = _intersection->is_hit;
        material  = _intersection->material;
    }

    void update(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _normal, float _distance, Material* _material) {
        position  = _position;
        direction = _direction;
        normal    = _normal;
        distance  = _distance;
        is_hit    = true;
        material  = _material;
    }
};

#endif
