#include "Scene3d.h"
#include "Sphere.h"

Scene3D::Scene3D() { objects.push_back(make_shared<Sphere>(Sphere())); }

const vector<shared_ptr<Shape>> &Scene3D::getObjects() { return objects; }

void Scene3D::draw() {
  for (auto shape : objects)
      shape->draw();
}

void Scene3D::drawWithNames()
{
    for (auto shape : objects)
        shape->drawWithName();
}
