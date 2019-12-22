#ifndef SKIPPYPIPELINE_H
#define SKIPPYPIPELINE_H

#include "Camera.h"
#include "Utils.h"

#include <QPoint>
#include <vector>

using namespace std;
using namespace skippy;

class SkippyPipeline {
public:
  SkippyPipeline();

  void addSketchPoint(const QPoint &point, const Camera *camera);
  void addToOnSequence(const PointSequence &point);
  void drawInputSkechesPoint();
  void drawInputRays();
  void drawOnSequence();

private:
  vector<QPoint> inputSketchedPoints;
  vector<qglviewer::Vec> projectedInptSketchedpoints;
  vector<Ray> inputRays;
  vector<QPoint> resampledInputPoints;
  PointsSequence onSequence;
};

#endif // SKIPPYPIPELINE_H
