#include "SkippyPipeline.h"
#include "gl/GLUtilityMethods.h"

#include <iostream>

SkippyPipeline::SkippyPipeline() {}

void SkippyPipeline::addSketchPoint(const QPoint &point, const Camera *camera) {
  inputSketchedPoints.push_back(point);
  bool found;
  projectedInptSketchedpoints.push_back(camera->pointUnderPixel(point, found));
}

void SkippyPipeline::drawInputSkechesPoint() {

  for (auto point : projectedInptSketchedpoints) {
    BasicGL::drawSphere(point.x, point.y, point.z, 0.3f,
                        BasicGL::optimalSlices(0.3f, 0.5f),
                        BasicGL::optimalStacks(0.5f, 0.5f));
  }
}
