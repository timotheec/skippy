#ifndef SHPERE_H
#define SHPERE_H

#include "Shape.h"
#include "Utils.h"

#include <iostream>
#include <qglviewer.h>

class Sphere : public Shape {
public:
  Sphere(qglviewer::Vec center = qglviewer::Vec(0, 0, 0), double radius = 1.0);

  void draw() const override;
  double distanceToLine(const skippy::Ray &ray) const;

  /**
   * Give the intersections of the ray with the sphere surface
   * @param ray the ray to test for intersction with the sphere
   * @param p1 point where the first intersection is store
   * @param p2 point where the second intersection is store
   * @return false if there is no intersection, true if there is one at least
   * (note : if there is one intersection p1 = p2)
   */
  bool intersect(const skippy::Ray &ray, qglviewer::Vec &p1, qglviewer::Vec &p2) const;

  qglviewer::Vec getCenter() const { return center; }
  void setCenter(qglviewer::Vec c) { center = c; }

  friend ostream &operator<<(ostream &os, const Sphere &sphere);

private:
  double radius;
  qglviewer::Vec center;
};

#endif // SHPERE_H
