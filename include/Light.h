#ifndef __LIGHT_H__
#define __LIGHT_H__

class Light {
public:
    glm::vec3 location;
    glm::vec3 color;
    bool is_directional = false;
    glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);

    Light(glm::vec3 _location, glm::vec3 _color, bool _is_directional, glm::vec3 _attenuation) {
        location = _location;
        color    = _color;
        is_directional = _is_directional;

        if (not is_directional) {
            attenuation = _attenuation;
        }
    }

    glm::vec3 getDirection(glm::vec3 position) {
        if (is_directional) {
            return glm::normalize(location);
        } else {
            return glm::normalize(location - position);
        }
    }
};

#endif