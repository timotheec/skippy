#ifndef SCENE3D_H
#define SCENE3D_H

#include "Shape.h"

#include <memory>
#include <vector>

class Scene3D {
public:
  Scene3D();
  const vector<shared_ptr<Shape>> &getObjects();
  void draw();
  void drawWithNames();

private:
  vector<shared_ptr<Shape>> objects;
};

#endif // SCENE3D_H
