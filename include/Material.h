#ifndef __MATERIAL_H__
#define __MATERIAL_H__

struct Material {
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 emission = glm::vec3(0.0f, 0.0f, 0.0f);
    float shininess = 0.0f;

    Material(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _emission, float _shininess) : 
    ambient(_ambient), diffuse(_diffuse), specular(_specular), emission(_emission), shininess(_shininess) {}
};

#endif 