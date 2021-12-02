#include <vector>
#include <FreeImage.h>

#include "Camera.h"
#include "Geometry.h"
#include "Intersection.h"

#ifndef __SCENE_H__
#define __SCENE_H__

typedef std::vector<std::vector<glm::vec3>> Image;

class Scene {
public:
    int width;
    int height;
    int max_depth = 5;
    const char* filename = "./output/raytrace.png";

    Camera* camera;
    std::vector<glm::vec3> vertices;
    std::vector<Geometry*> geometries;

    Ray rayThruPixel(int i, int j);
    Intersection intersect(Ray* ray);
    glm::vec3 findColor(Intersection* hit);
    Image rayTrace();

    ~Scene() {
        delete camera;

        for (int i = 0; i < geometries.size(); i++) {
            delete geometries[i];
        }
    }
};

#endif
