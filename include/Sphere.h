#include "Geometry.h"

#ifndef __SPHERE_H__
#define __SPHERE_H__

class Sphere : public Geometry {
public:
    glm::vec3 center;
    float radius;
    glm::mat4 transform;

    Intersection intersect(Ray* ray) {
        Intersection hit;

        // Transform ray to model coordinates
        glm::vec3 model_point = glm::vec3(glm::inverse(transform) * glm::vec4(ray->point, 1.0f));
        glm::mat3 A = glm::mat3(transform);
        glm::vec3 model_direction = glm::normalize(glm::inverse(A) * ray->direction);

        // Find t
        glm::vec3 diff = model_point - center;
        float model_t = glm::dot(-1.0f * model_direction, diff) - sqrt( pow(glm::dot(model_direction, diff),2) - pow(glm::length(diff), 2) + pow(radius,2) );

        if (model_t > 0) {
            // Find intersection
            glm::vec3 model_position = model_point + model_t * model_direction;
            glm::vec3 model_normal = glm::normalize(model_position - center);

            // Transform intersection back to world coordinates
            glm::vec3 position = glm::vec3(transform * glm::vec4(model_position, 1.0f));
            glm::vec3 normal = glm::normalize(glm::inverse(glm::transpose(A)) * model_normal);
            float t = glm::length(position - ray->point);
            hit.update(position, -1.0f * ray->direction, normal, t, material);
        }
        
        return hit;
    }

    Sphere(float x, float y, float z, float r, glm::mat4 _transform, Material* _material) : 
           center(glm::vec3(x, y, z)), radius(r), transform(_transform) {
        material = _material;
    }
};

#endif