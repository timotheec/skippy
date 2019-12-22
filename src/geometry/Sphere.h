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

  qglviewer::Vec getCenter() const { return center; }
  void setCenter(qglviewer::Vec c) { center = c; }

  friend ostream &operator<<(ostream &os, const Sphere &sphere);

private:
  double radius;
  qglviewer::Vec center;
};

#endif // SHPERE_H
