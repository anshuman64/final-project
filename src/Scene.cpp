#define GLM_FORCE_RADIANS
#include <iostream>

#include "Scene.h"
#include "Intersection.h"

Ray Scene::rayThruPixel(int i, int j) {
    glm::vec3 point = camera->eye;

    float fovy_rad = camera->fovy * M_PI/180.0f;
    float fovy_tan = glm::tan(fovy_rad/2.0f);

    float a = 2 * (i + 0.5f) / width - 1;
    a = a * camera->aspect * fovy_tan;
    float b = 1 - 2 * (j + 0.5f) / height;
    b = b * fovy_tan;

    glm::vec3 direction = glm::normalize(a * glm::vec3(camera->cam[0]) + b * glm::vec3(camera->cam[1]) - glm::vec3(camera->cam[2]));

    Ray ray = Ray(point, direction);
    return ray;
}


Intersection Scene::intersect(Ray* ray) {
    float dist = std::numeric_limits<float>::infinity();
    Intersection hit;

    for (int i = 0; i < geometries.size(); i++) {
        Intersection hit_temp = geometries[i]->intersect(ray);

        if (hit_temp.is_hit && hit_temp.distance < dist) {
            dist = hit_temp.distance;
            hit = hit_temp;
        }
    }

    return hit;
}


glm::vec3 Scene::findColor(Intersection* hit, bool use_shadows, bool use_mirror, int mirror_depth) {
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 offset_hit_position;

    if (hit->is_hit) {
        // Add ambient + emission
        color = hit->material->ambient + hit->material->emission;

        for (int i = 0; i < lights.size(); i++) {
            glm::vec3 light_direction = lights[i]->getDirection(hit->position);
            float light_distance = glm::length(lights[i]->location - hit->position);
            Intersection shadow_hit;

            if (use_shadows) {
                // Cast shadow ray
                offset_hit_position = hit->position + 0.001f * light_direction; // handle z-fighting
                Ray shadow_ray = Ray(offset_hit_position, light_direction);
                shadow_hit = intersect(&shadow_ray);
            }
            
            if ( (lights[i]->is_directional && not shadow_hit.is_hit) || (not lights[i]->is_directional && shadow_hit.distance > light_distance) ) {
                // Add diffuse
                glm::vec3 light_contribution = hit->material->diffuse * std::max(glm::dot(hit->normal, light_direction), 0.0f);
                
                // Add specular
                glm::vec3 half_vector = glm::normalize(hit->direction + light_direction);
                light_contribution += hit->material->specular * std::pow(std::max(glm::dot(hit->normal, half_vector), 0.0f), hit->material->shininess);
                
                // Add attenuation
                float total_attenuation = lights[i]->attenuation.x + lights[i]->attenuation.y * light_distance + lights[i]->attenuation.z * std::pow(light_distance,2);
                light_contribution = lights[i]->color * light_contribution / total_attenuation;

                color += light_contribution;
            }
        }

        if (use_mirror && mirror_depth < max_depth) {
            glm::vec3 mirror_direction = 2 * glm::dot(hit->normal, hit->direction) * hit->normal - hit->direction;
            offset_hit_position = hit->position + 0.001f * mirror_direction;
            Ray mirror_ray = Ray(offset_hit_position, mirror_direction);
            Intersection mirror_hit = intersect(&mirror_ray);
            color += hit->material->specular * findColor(&mirror_hit, use_shadows, use_mirror, mirror_depth+1);
        }
    }

    return color;
}


Image Scene::rayTrace(bool use_shadows, bool use_mirror) {
    Image image;
    image.resize(width);

    for (int i = 0; i < width; i++) {
        image[i].resize(height);
    }

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Ray ray = rayThruPixel(i, j);
            Intersection hit = intersect(&ray);
            glm::vec3 color = findColor(&hit, use_shadows, use_mirror, 0);
            image[i][j] = 255.0f * color;
        }
    }

    return image;
}
