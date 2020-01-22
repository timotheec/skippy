#include "Sphere.h"
#include "gl/GLUtilityMethods.h"

using namespace std;
using namespace qglviewer;
using namespace skippy;

Sphere::Sphere(qglviewer::Vec center, double radius)
    : radius(radius), center(center) {}

void Sphere::draw() const {
  // TODO: aptimal slice, what is exactly ?
  BasicGL::drawSphere(center.x, center.y, center.z, radius,
                      BasicGL::optimalSlices(radius, 0.5f),
                      BasicGL::optimalStacks(radius, 0.5f));
}

double Sphere::distanceToLine(const skippy::Ray &ray) const {
  qglviewer::Vec ac = center - ray.orig;
  double adNorm = ac * ray.dir;
  return sqrt(pow(ac.norm(), 2) - pow(adNorm, 2)) - radius;
}

bool Sphere::intersect(const skippy::Ray &ray, qglviewer::Vec &p1,
                       qglviewer::Vec &p2) const {
  // code source adapted form :
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

  double t0, t1;                    // solutions for t if the ray intersects
  double radius2 = radius * radius; // square sphere radius

  // geometric solution
  Vec L = center - ray.orig;
  double tca = L * ray.dir;

  double d2 = (L * L) - tca * tca;
  if (d2 > radius2)
    return false;
  double thc = sqrt(radius2 - d2);
  t0 = tca - thc;
  t1 = tca + thc;

  if (t0 > t1)
    std::swap(t0, t1);

  if (t0 < 0) {
    t0 = t1; // if t0 is negative, let's use t1 instead
    if (t0 < 0)
      return false; // both t0 and t1 are negative
  }

  p1 = ray.orig + t0 * ray.dir;
  p2 = ray.orig + t1 * ray.dir;

  return true;
}

ostream &operator<<(ostream &os, const Sphere &sphere) {
  return os << "{ centre: " << sphere.center << " "
            << "radius: " << sphere.radius << " }";
}
