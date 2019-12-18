#ifndef SKIPPYPIPELINE_H
#define SKIPPYPIPELINE_H

#include "Camera.h"
#include "Utils.h"

#include <QPoint>
#include <vector>

using namespace std;

class SkippyPipeline {
public:
  SkippyPipeline();

  void addSketchPoint(const QPoint &point, const Camera *camera);
  void drawInputSkechesPoint();
  void drawInputRays();

private:
  vector<QPoint> inputSketchedPoints;
  vector<qglviewer::Vec> projectedInptSketchedpoints;
  vector<skippy::Ray> inputRays;
  vector<QPoint> resampledInputPoints;
};

#endif // SKIPPYPIPELINE_H
