#include "Sphere.h"
#include "gl/GLUtilityMethods.h"

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

ostream &operator<<(ostream &os, const Sphere &sphere) {
  return os << "{ centre: " << sphere.center << " "
            << "radius: " << sphere.radius << " }";
}
