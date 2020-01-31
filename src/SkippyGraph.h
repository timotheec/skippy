#ifndef SKYPPYGRAPH_H
#define SKYPPYGRAPH_H

#include "Utils.h"

using namespace skippy;
using namespace std;

class SkippyGraph {
public:
  SkippyGraph(const PointsSequence &onCandidates,
              const PointsSequence &offRays  = PointsSequence());
  void print() const; // For debugging purpose

private:
  void buildOnSegments(const PointsSequence &onCandidates);
  vector<OnSegment> onSegments;
};

#endif // SKIPPYGRAPH_H
