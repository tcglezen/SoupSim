#include "iostream"
#include <nanogui/nanogui.h>

#include "../fluidMesh.h"
#include "../fluidSimulator.h"
#include "plane.h"

using namespace std;
using namespace CGL;

// Original off set is 0.0001
#define SURFACE_OFFSET 0.00001

void Plane::collide(Particle &p) {
  // TODO (Part 3): Handle collisions with planes.
  // If last pos is on one side of plane and pos is on other side, then we have a collision
  if ((dot(p.position - point, normal) >= 0) != (dot(p.last_position - point, normal) >= 0)) { //TODO: this iffy if condition is probably causing bouncing
      // First project position onto plane (source: https://www.youtube.com/watch?v=r5VCChxnLnQ)
      Vector3D v = p.position - point;
      Vector3D proj_n_v = dot(v, normal) * normal;
      Vector3D tangent_point = p.position - proj_n_v;

      // compute correction vector for last_position
      Vector3D correction_vector = (tangent_point + normal * SURFACE_OFFSET) - p.last_position;

      // set point mass's new position to be it's last position adjusted by above correction vector scaled down by friction
      p.position = p.last_position + correction_vector * (1.0 - friction);
  }
}

void Plane::render(GLShader &shader) {
  nanogui::Color color(0.7f, 0.7f, 0.7f, 1.0f);

  Vector3f sPoint(point.x, point.y, point.z);
  Vector3f sNormal(normal.x, normal.y, normal.z);
  Vector3f sParallel(normal.y - normal.z, normal.z - normal.x,
                     normal.x - normal.y);
  sParallel.normalize();
  Vector3f sCross = sNormal.cross(sParallel);

  MatrixXf positions(3, 4);
  MatrixXf normals(3, 4);

  positions.col(0) << sPoint + 2 * (sCross + sParallel);
  positions.col(1) << sPoint + 2 * (sCross - sParallel);
  positions.col(2) << sPoint + 2 * (-sCross + sParallel);
  positions.col(3) << sPoint + 2 * (-sCross - sParallel);

  normals.col(0) << sNormal;
  normals.col(1) << sNormal;
  normals.col(2) << sNormal;
  normals.col(3) << sNormal;

  if (shader.uniform("u_color", false) != -1) {
    shader.setUniform("u_color", color);
  }
  shader.uploadAttrib("in_position", positions);
  if (shader.attrib("in_normal", false) != -1) {
    shader.uploadAttrib("in_normal", normals);
  }

  shader.drawArray(GL_TRIANGLE_STRIP, 0, 4);
}
