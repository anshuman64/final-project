#define GLM_FORCE_RADIANS
#include <iostream>
#include <cmath>

#include "Scene.h"
#include "Intersection.h"


Ray Scene::rayThruPixel(int i, int j, bool use_gi) {
    glm::vec3 point = camera->eye;

    float fovy_rad = camera->fovy * M_PI/180.0f;
    float fovy_tan = glm::tan(fovy_rad/2.0f);

    float delta_A = 0.5f;
    float delta_B = 0.5f;
    if (use_gi) {
        // Use random point in pixel for GI
        delta_A = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); 
        delta_B = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); 
    }

    float a = 2 * (i + delta_A) / width - 1;
    a = a * camera->aspect * fovy_tan;
    float b = 1 - 2 * (j + delta_B) / height;
    b = b * fovy_tan;

    glm::vec3 direction = glm::normalize(a * glm::vec3(camera->camera_matrix[0]) + b * glm::vec3(camera->camera_matrix[1]) - glm::vec3(camera->camera_matrix[2]));

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


glm::vec3 Scene::calculate_hit_color(Intersection* hit, std::string color_type, bool use_shadows) {
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < lights.size(); i++) {
        glm::vec3 light_direction = lights[i]->getDirection(hit->position);
        float light_distance = glm::length(lights[i]->location - hit->position);
        
        Intersection shadow_hit;
        if (use_shadows) {
            Ray shadow_ray = hit->get_shadow_ray(light_direction);
            shadow_hit = intersect(&shadow_ray);
        }
        
        if (not (use_shadows && shadow_hit.is_shadow(light_distance, lights[i]->is_directional)) ) {
            glm::vec3 light_contribution = glm::vec3(0.0f, 0.0f, 0.0f);

            if (color_type == "diffuse") {
                light_contribution += hit->calculate_diffuse(light_direction);
            } else if (color_type == "specular") {
                light_contribution += hit->calculate_specular(light_direction);
            } else {
                light_contribution += hit->calculate_diffuse(light_direction) + hit->calculate_specular(light_direction);
            }

            light_contribution *= lights[i]->calculate_intensity(light_distance);
            color += light_contribution;
        }
    }

    return color;
}


glm::vec3 Scene::findColor(Intersection* hit, bool use_shadows, bool use_mirror, int mirror_depth) {
    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    if (hit->is_hit) {
        // Add ambient + emission
        color = hit->material->ambient + hit->material->emission;
        color += calculate_hit_color(hit, "both", use_shadows);

        if (use_mirror && mirror_depth < max_depth) {
            // Cast mirror ray
            Ray mirror_ray = hit->get_mirror_ray();
            Intersection mirror_hit = intersect(&mirror_ray);

            color += hit->material->specular * findColor(&mirror_hit, use_shadows, use_mirror, mirror_depth+1);
        }
    }

    return color;
}


glm::vec3 Scene::russianRoulette(int i, int j, int K, float lambda, bool use_shadows) {
    glm::vec3 color_sum = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 offset_hit_position;

    for (int k = 0; k < K; k++) {
        glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 total_weight = glm::vec3(1.0f, 1.0f, 1.0f);
        float factor = 1.0f;
        int path_length = 1;

        Ray ray = rayThruPixel(i, j, true);

        while (true) {
            Intersection hit = intersect(&ray);

            if (not hit.is_hit) {
                break;
            }

            bool terminate = rand() < (RAND_MAX * lambda);

            if (terminate) {
                factor *= lambda;
                color += total_weight * (hit.material->emission + calculate_hit_color(&hit, "diffuse", use_shadows));
                break;
            } else {
                factor *= (1-lambda);
                color += total_weight * hit.material->emission;

                if (rand() < (RAND_MAX * 0.5f)) {
                    // If diffuse...
                    total_weight *= hit.material->diffuse;
                    ray = hit.get_diffuse_ray();
                } else {
                    // Else if specular...
                    total_weight *= hit.material->specular;
                    ray = hit.get_mirror_ray();
                }
            }
        }

        color_sum += color / factor;
    }

    color_sum /= K;

    return color_sum;
}


Image Scene::rayTrace(bool use_shadows, bool use_mirror, bool use_gi, int K, float lambda) {
    Image image;
    glm::vec3 color;

    image.resize(width);
    for (int i = 0; i < width; i++) {
        image[i].resize(height);
    }

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (not use_gi) {
                Ray ray = rayThruPixel(i, j, false);
                Intersection hit = intersect(&ray);
                color = findColor(&hit, use_shadows, use_mirror, 0);
            } else {
                color = russianRoulette(i, j, K, lambda, use_shadows);
            }

            image[i][j] = 255.0f * color;
        }
    }

    return image;
}
