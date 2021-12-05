#include <vector>
#include <FreeImage.h>

#include "Camera.h"
#include "Geometry.h"
#include "Intersection.h"
#include "Light.h"

#ifndef __SCENE_H__
#define __SCENE_H__

typedef std::vector<std::vector<glm::vec3>> Image;

class Scene {
public:
    int width;
    int height;
    int max_depth = 5;
    std::string filename = "./output/raytrace.png";

    Camera* camera;
    std::vector<glm::vec3> vertices;
    std::vector<Geometry*> geometries;
    std::vector<Light*> lights;

    Ray rayThruPixel(int i, int j, bool use_gi);
    Intersection intersect(Ray* ray);

    glm::vec3 calculate_hit_color(Intersection* hit, std::string color_type, bool use_shadows);
    glm::vec3 findColor(Intersection* hit, bool use_shadows, bool use_mirror, int mirror_depth);
    glm::vec3 russianRoulette(int i, int j, int K, float lambda, bool use_shadows);

    Image rayTrace(bool use_shadows, bool use_mirror, bool use_gi, int K, float lambda);

    ~Scene() {
        delete camera;

        for (int i = 0; i < geometries.size(); i++) {
            delete geometries[i];
        }
    }
};

#endif
