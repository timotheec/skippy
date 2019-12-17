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

void SkippyPipeline::drawInputSkechesPoint() {
  for (auto point : projectedInptSketchedpoints) {
    BasicGL::drawSphere(point.x, point.y, point.z, 0.3f,
                        BasicGL::optimalSlices(0.3f, 0.5f),
                        BasicGL::optimalStacks(0.5f, 0.5f));
  }
}

void SkippyPipeline::drawInputRays()
{
    for(auto ray : inputRays) {
        qglviewer::Vec dest = 5.0 * ray.dir;
        glBegin(GL_LINES);
        glVertex3d(ray.orig.x, ray.orig.y, ray.orig.z);
        glVertex3d(dest.x, dest.y, dest.z);
        glEnd();
    }
}
