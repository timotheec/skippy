#include "SkippyPipeline.h"
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

void SkippyPipeline::drawInputSkechesPoint() {
  for (auto point : projectedInptSketchedpoints) {
    BasicGL::drawSphere(point.x, point.y, point.z, 0.3f,
                        BasicGL::optimalSlices(0.3f, 0.5f),
                        BasicGL::optimalStacks(0.5f, 0.5f));
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
    BasicGL::drawSphere(pointSeq.pos.x, pointSeq.pos.y, pointSeq.pos.z, 0.3f,
                        BasicGL::optimalSlices(0.3f, 0.5f),
                        BasicGL::optimalStacks(0.5f, 0.5f));
  }
}
