#include "Intersection.h"
#include "Material.h"

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

class Geometry {
public:
    Material* material;
    virtual Intersection* intersect(Ray* ray) = 0;
    
    virtual ~Geometry() {
        delete material;
    };
};

#endif
