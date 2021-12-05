#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "Intersection.h"

void Intersection::update(glm::vec3 _position, glm::vec3 _direction, glm::vec3 _normal, float _distance, Material* _material) {
    position  = _position;
    direction = _direction;
    normal    = _normal;
    distance  = _distance;
    is_hit    = true;
    material  = _material;
}

bool Intersection::is_shadow(float light_distance, bool is_directional) {
    return (is_directional && is_hit) || (not is_directional && distance < light_distance);
}

Ray Intersection::get_shadow_ray(glm::vec3 light_direction) {
    glm::vec3 offset_hit_position = position + 0.001f * light_direction; // handle z-fighting
    Ray shadow_ray = Ray(offset_hit_position, light_direction); 

    return shadow_ray;
}

Ray Intersection::get_mirror_ray() {
    glm::vec3 mirror_direction = 2 * glm::dot(normal, direction) * normal - direction;
    glm::vec3 offset_hit_position = position + 0.001f * mirror_direction;
    
    Ray mirror_ray = Ray(offset_hit_position, mirror_direction);
    return mirror_ray;
}

Ray Intersection::get_diffuse_ray() {
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

    // Generate sample
    float s = rand() / RAND_MAX;
    float t = rand() / RAND_MAX;

    float u = 2 * M_PI * s;
    float v = sqrt(1 - t);

    glm::vec3 diffuse_direction = glm::normalize(glm::vec3(v * cos(u), t, v * sin(u)));

    // If normal & up are in same direction...
    if (normal == -1.0f * up) {
        diffuse_direction = -1.0f * diffuse_direction;
    } else if (normal != up) {
        // Rotate sample
        glm::vec3 axis = glm::cross(normal, up);
        float angle = acos(glm::dot(normal, up));
        glm::mat4 rotateMatrix = glm::rotate(glm::mat4(), angle, axis);

        diffuse_direction = glm::normalize(glm::vec3(rotateMatrix * glm::vec4(diffuse_direction, 0.0f)));
    }

    glm::vec3 offset_hit_position = position + 0.001f * diffuse_direction;
    Ray diffuse_ray = Ray(offset_hit_position, diffuse_direction);
    
    return diffuse_ray;
}

glm::vec3 Intersection::calculate_diffuse(glm::vec3 light_direction) {
    glm::vec3 diffuse_color = material->diffuse * std::max(glm::dot(normal, light_direction), 0.0f);
    return diffuse_color;
}

glm::vec3 Intersection::calculate_specular(glm::vec3 light_direction) {
    glm::vec3 half_vector = glm::normalize(direction + light_direction);
    glm::vec3 specular_color = material->specular * std::pow(std::max(glm::dot(normal, half_vector), 0.0f), material->shininess);
    
    return specular_color;
}