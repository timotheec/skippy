#ifndef SKYPPYGRAPH_H
#define SKYPPYGRAPH_H

#include "Utils.h"

using namespace skippy;
using namespace std;

struct SkippyNode {
  OnSegment *onSegment;
  vector<SkippyNode *> adjacency;

  friend std::ostream &operator<<(std::ostream &out, const SkippyNode &node) {
    out << "{ " << *node.onSegment << ", {";
    for (auto &adjNode : node.adjacency)
      out << *(adjNode->onSegment) << ",";
    return out << " } }";
  }
};

class SkippyGraph {
public:
  SkippyGraph(const PointsSequence &onCandidates, const vector<Ray> &inputRays);
  void drawPath() const;
  void changePath();
  void print() const; // For debugging purpose
  const vector<OffSegment> &getOffSegment() const;

private:
  void buildOnSegments(const PointsSequence &onCandidates);
  void connectOnSegments();
  void createNodes();
  void findAllLongestPaths();
  void findAllPaths(const SkippyNode &node, vector<OnSegment *> path);
  void computeOffSegments();

  uint pathIdSeclected = 0;
  vector<Ray> inputRays;
  vector<OnSegment> onSegments;
  vector<OffSegment> offSegments;
  vector<SkippyNode> nodes;
  vector<vector<OnSegment *>> paths;
};

#endif // SKIPPYGRAPH_H
