#include "Intersection.h"

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

class Geometry {
public:
    virtual Intersection intersect(Ray* ray) = 0;
    virtual ~Geometry() {};
};

#endif
