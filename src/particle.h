#ifndef PARTICLE_H
#define PARTICLE_H

#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "CGL/vector3D.h"

using namespace CGL;

// Forward declarations
class Halfedge;

struct Particle {
    Particle(Vector3D position)
            : start_position(position), position(position),
              last_position(position) {}

    Vector3D normal();
    Vector3D velocity(double delta_t) {
        return (position - last_position) / delta_t;
    }

    // static values
    Vector3D start_position;

    // dynamic values
    Vector3D position;
    Vector3D last_position;
    Vector3D forces;
    // mesh reference
    Halfedge *halfedge;
};

#endif /* PARTICLE_H */
