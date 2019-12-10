#include "SkippyPipeline.h"

#include <iostream>

SkippyPipeline::SkippyPipeline() {}

void SkippyPipeline::addSketchPoint(const QPoint &point) {
    inputSketchesPoints.push_back(point);
}

void SkippyPipeline::drawInputSkechesPoint() {
  for (auto point : inputSketchesPoints)
    cout << "{" << point.x() << " , " << point.y() << "}" << ' ';
  cout << endl;
}
