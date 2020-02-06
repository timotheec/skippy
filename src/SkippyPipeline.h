#ifndef SKIPPYPIPELINE_H
#define SKIPPYPIPELINE_H

#include "Camera.h"
#include "SkippyGraph.h"
#include "Utils.h"

#include <QPoint>
#include <utility>
#include <vector>

using namespace std;
using namespace skippy;

class SkippyPipeline {
public:
  SkippyPipeline();
  ~SkippyPipeline();

  void addSketchPoint(const QPoint &point, const Camera *camera);
  void addToInputRays(const Ray &ray);
  void addToOnSequence(const PointSequence &point);
  void addToOffSequence(const PointSequence &point);

  void drawInputSkechesPoint();
  void drawInputRays();
  void drawOnSequence();
  void drawOnCandidates();
  void drawFinalOffPoints();

  // Temporary method to update the max heigth of the off sequence (could be 0
  // or 1 for now)
  void updateOffMaxHeight(double height, unsigned int noSeq);
  void computeOnPointsHeigth();

  void computeOnCandidates();
  void buildGraph();

private:
  vector<QPoint> inputSketchedPoints;
  vector<qglviewer::Vec> projectedInptSketchedpoints;
  vector<Ray> inputRays;
  vector<QPoint> resampledInputPoints;
  PointsSequence onSequence;
  PointsSequence offSequence;
  PointsSequence onCandidates;

  SkippyGraph *skippyGraph = nullptr;

  // Temporary attribute to keep the max heigths of the off points for the
  // sequence 0 and 1
  pair<double, double> offMaxHeigths = make_pair(0.0, 0.0);
};

#endif // SKIPPYPIPELINE_H
