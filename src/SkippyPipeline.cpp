#include "SkippyPipeline.h"
#include "geometry/Sphere.h"
#include "gl/GLUtilityMethods.h"

#include <iostream>

using namespace skippy;

SkippyPipeline::SkippyPipeline() {}

void SkippyPipeline::addSketchPoint(const QPoint &point, const Camera *camera) {
  inputSketchedPoints.push_back(point);
  bool found;
  projectedInptSketchedpoints.push_back(camera->pointUnderPixel(point, found));

  Ray rayUnderMouse;
  camera->convertClickToLine(point, rayUnderMouse.orig, rayUnderMouse.dir);
  inputRays.push_back(rayUnderMouse);
}

void SkippyPipeline::addToOnSequence(const PointSequence &point) {
  onSequence.addPoint(point);
}

void SkippyPipeline::addToOffSequence(const PointSequence &point) {
  offSequence.addPoint(point);
}

void SkippyPipeline::drawInputSkechesPoint() {
  for (auto point : projectedInptSketchedpoints) {
    Sphere sphere(point, 0.3);
    sphere.draw();
  }
}

void SkippyPipeline::drawInputRays() {
  const double DISPLAYED_RAY_LENGHT = 20.0;
  for (auto ray : inputRays) {
    qglviewer::Vec dest = ray.orig + DISPLAYED_RAY_LENGHT * ray.dir;
    glBegin(GL_LINES);
    glVertex3dv(ray.orig);
    glVertex3dv(dest);
    glEnd();
  }
}

void SkippyPipeline::drawOnSequence() {
  const double DISPLAYED_RAY_LENGHT = 20.0;
  for (auto pointSeq : onSequence.pointsSeq) {
    qglviewer::Vec dest =
        pointSeq.ray.orig + DISPLAYED_RAY_LENGHT * pointSeq.ray.dir;
    glBegin(GL_LINES);
    glVertex3dv(pointSeq.ray.orig);
    glVertex3dv(dest);
    glEnd();
    Sphere sphere(pointSeq.pos, 0.3);
    sphere.draw();
  }
}

void SkippyPipeline::updateOffMaxHeight(double heigth, unsigned int noSeq) {
  if (noSeq == 0 && heigth > offMaxHeigths.first)
    offMaxHeigths.first = heigth;
  else if (noSeq == 1 && heigth > offMaxHeigths.second)
    offMaxHeigths.second = heigth;
  else if (noSeq > 1) {
    cout << "Sequence numero have to be 0 or 1." << endl;
    exit(EXIT_FAILURE);
  }
}
