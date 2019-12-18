#ifndef SHPERE_H
#define SHPERE_H

#include "Shape.h"

#include <iostream>
#include <qglviewer.h>

class Sphere : public Shape {
public:
  Sphere(qglviewer::Vec center = qglviewer::Vec(0, 0, 0), double radius = 1.0);

  void draw() const override;

  qglviewer::Vec getCenter() const { return center; }
  void setCenter(qglviewer::Vec c) { center = c; }

  double radius;
  qglviewer::Vec center;

  friend ostream &operator<<(ostream &os, const Sphere &sphere);
};

#endif // SHPERE_H
