#include <nanogui/nanogui.h>

#include "../fluidMesh.h"
#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace nanogui;
using namespace CGL;

void Sphere::collide(Particle &p) {
  // TODO (Part 3): Handle collisions with spheres.
  // Calculate extending path vector
  Vector3D pos = p.position - origin;
  // Get distance of that
  double path_norm = pos.norm();

  // Get direction of vector
  Vector3D dir = pos;
  dir.normalize();

  // Get surface intersection point (tangent point)
  Vector3D tangent_point = dir * radius;
  double tangent_norm = tangent_point.norm();

  if (path_norm <= tangent_norm) {
      // we have a collision

      // Get norm of last pos relative to origin
      double last_pos_norm = (p.last_position - origin).norm();

      // Correction vector moves last position to the tangent point
      Vector3D correction_vector = dir * (tangent_norm - last_pos_norm);

      // Apply correction vector to last position scaled by friction, then set to position
      p.position = p.last_position + correction_vector * (1.0 - friction);
  }

}

void Sphere::render(GLShader &shader) {
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
  m_sphere_mesh.draw_sphere(shader, origin, radius * 0.92);
}
