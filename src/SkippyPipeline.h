#ifndef SKIPPYPIPELINE_H
#define SKIPPYPIPELINE_H

#include <QPoint>
#include <vector>

using namespace std;

class SkippyPipeline {
public:
  SkippyPipeline();

  void addSketchPoint(const QPoint &point);
  void drawInputSkechesPoint();

private:
  vector<QPoint> inputSketchesPoints;
  vector<QPoint> resampledInputPoints;
};

#endif // SKIPPYPIPELINE_H
