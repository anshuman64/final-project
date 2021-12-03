#include "Geometry.h"

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

class Triangle : public Geometry {
public:
    glm::vec3 vertices[3];
    glm::vec3 normals[3];

    Intersection intersect(Ray* ray) {
        Intersection hit;

        // Determine intersection
        glm::mat4 A;
        A[0] = glm::vec4(vertices[0], 1.0f);
        A[1] = glm::vec4(vertices[1], 1.0f);
        A[2] = glm::vec4(vertices[2], 1.0f);
        A[3] = glm::vec4(-1.0f * ray->direction, 0.0f);

        glm::vec4 y = glm::vec4(ray->point, 1.0f);
        glm::vec4 x = glm::inverse(A)*y;

        // Update hit if valid intersection
        if (x.x >= 0 && x.y >= 0 && x.z >= 0 && x.w >= 0) {
            glm::vec3 position = x.x * vertices[0] + x.y * vertices[1] + x.z * vertices[2];
            glm::vec3 normal = glm::normalize(x.x * normals[0] + x.y * normals[1] + x.z * normals[2]);

            hit.update(position, -1.0f * ray->direction, normal, x.w);
        }

        return hit;
    }

    Triangle(glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2, glm::mat4 transform) {
        // Transform and store each vertex
        vertices[0] = glm::vec3(transform * glm::vec4(vertex0, 1.0f));
        vertices[1] = glm::vec3(transform * glm::vec4(vertex1, 1.0f));
        vertices[2] = glm::vec3(transform * glm::vec4(vertex2, 1.0f));

        // Calculate normals for each vertex
        glm::vec3 edge1 = vertices[1]-vertices[0];
        glm::vec3 edge2 = vertices[2]-vertices[0];
        glm::vec3 edge3 = vertices[2]-vertices[1];

        normals[0] = glm::normalize(glm::cross(edge1, edge2));
        normals[1] = glm::normalize(glm::cross(edge1, edge3));
        normals[2] = glm::normalize(glm::cross(edge2, edge3));
    }
};

#endif