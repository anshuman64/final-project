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
    // Geometry  geometry;
    float     distance;
    bool      is_hit = false;

    void update(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _normal, float _distance) {
        position = _position;
        direction = _direction;
        normal = _normal;
        distance = _distance;
        is_hit = true;
    }
};

#endif
