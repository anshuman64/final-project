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


glm::vec3 Scene::findColor(Intersection* hit) {
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    if (hit->is_hit) {
        // Add ambient + emission
        color = hit->material->ambient + hit->material->emission;

        for (int i = 0; i < lights.size(); i++) {
            // Cast shadow ray
            glm::vec3 l = lights[i]->getDirection(hit->position);
            glm::vec3 offset_position = hit->position + glm::vec3(0.0f, 0.0f, 0.001f); // handle z-fighting
            Ray ray = Ray(offset_position, l);
            Intersection shadow_hit = intersect(&ray);

            if (not shadow_hit.is_hit || not is_shadows) {
                // Add diffuse
                glm::vec3 light_contribution = hit->material->diffuse * std::max(glm::dot(hit->normal, l), 0.0f);
                
                // Add specular
                glm::vec3 h = glm::normalize(hit->direction + l);
                light_contribution += hit->material->specular * std::pow(std::max(glm::dot(hit->normal, h), 0.0f), hit->material->shininess);
                
                // Add attenuation
                float r = glm::length(lights[i]->location - hit->position);
                float a = lights[i]->attenuation.x + lights[i]->attenuation.y * r + lights[i]->attenuation.z * std::pow(r,2);
                light_contribution = light_contribution / a;

                color += light_contribution;
            }
        }

        color = 255.0f * color;
    }

    return color;
}


Image Scene::rayTrace() {
    Image image;
    image.resize(width);

    for (int i = 0; i < width; i++) {
        image[i].resize(height);
    }

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Ray ray = rayThruPixel(i, j);
            Intersection hit = intersect(&ray);
            glm::vec3 color = findColor(&hit);
            image[i][j] = color;
        }
    }

    return image;
}
