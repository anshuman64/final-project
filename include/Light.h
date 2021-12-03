#ifndef __LIGHT_H__
#define __LIGHT_H__

class Light {
public:
    glm::vec3 location;
    glm::vec3 color;
    glm::vec3 attenuation;

    virtual glm::vec3 getDirection(glm::vec3 position) = 0;
    
    virtual ~Light() {};
};

class PointLight : public Light {
public:
    PointLight(glm::vec3 _location, glm::vec3 _color) {
        location = _location;
        color = _color;
        attenuation = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 getDirection(glm::vec3 position) {
        return glm::normalize(location - position);
    }
};

class DirectionalLight : public Light {
public:
    DirectionalLight(glm::vec3 _location, glm::vec3 _color) {
        location = _location;
        color = _color;
        attenuation = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glm::vec3 getDirection(glm::vec3 position) {
        return location;
    }
};

#endif