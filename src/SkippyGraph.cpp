#include "SkippyGraph.h"
#include "geometry/Sphere.h"
#include <algorithm>
#include <iostream>

SkippyGraph::SkippyGraph(const PointsSequence &onCandidates,
                         const vector<Ray> &inputRays)
    : inputRays(inputRays) {
  buildOnSegments(onCandidates);
  createNodes();
  connectOnSegments();
  findAllLongestPaths();
  computeOffSegments();
}

void SkippyGraph::drawPath() const {
  if (paths.empty())
    return;
  const vector<OnSegment *> &path = paths[pathIdSeclected];

  glColor3f(0.5f, 0.2f, 0.6f);
  Sphere sphere(qglviewer::Vec(0, 0, 0), 0.3);

  for (auto &onSegment : path)
    for (auto &onVertex : onSegment->vertices.pointsSeq) {
      sphere.setCenter(onVertex.pos);
      sphere.draw();
    }
  for (auto &offSegment : offSegments)
    for (auto &offVertex : offSegment.vertices.pointsSeq) {
      sphere.setCenter(offVertex.pos);
      sphere.draw();
    }
}

void SkippyGraph::changePath() {
  ++pathIdSeclected %= paths.size();
  computeOffSegments();
}

void SkippyGraph::print() const {
  //  for (const auto &onSegment : nodes) {
  //    cout << onSegment << endl;
  //  }
  //  for (auto &path : paths) {
  //    for (auto &onSegment : path)
  //      cout << *onSegment;
  //    cout << endl;
  //  }
}

const vector<OffSegment> &SkippyGraph::getOffSegment() const {
  return offSegments;
}

void SkippyGraph::buildOnSegments(const PointsSequence &onCandidates) {
  auto it = begin(onCandidates.pointsSeq);
  OnSegment firstOnSegment = {
      {{*it}}, it->intersectOrder, it->ray.index, it->ray.index};
  onSegments.push_back(firstOnSegment);
  ++it;

  for (auto end = std::end(onCandidates.pointsSeq); it != end; ++it) {
    bool candidateAdded = false;
    for (auto &onSegment : onSegments) {
      int intersectOrder = onSegment.intersectOrder;
      int endRayIndex = onSegment.endVertex;
      if (it->intersectOrder == intersectOrder &&
          it->ray.index == endRayIndex + 1) {
        onSegment.vertices.addPoint(*it);
        onSegment.endVertex = it->ray.index;
        candidateAdded = true;
        break;
      }
    }
    if (!candidateAdded) {
      OnSegment onSegment = {
          {{*it}}, it->intersectOrder, it->ray.index, it->ray.index};
      onSegments.push_back(onSegment);
    }
  }
}

void SkippyGraph::connectOnSegments() {
  for (uint i = 0; i < nodes.size(); i++)
    for (uint j = 0; j < nodes.size(); j++) {
      if (i == j)
        continue;
      if (nodes[j].onSegment->startVertex + 1 > nodes[i].onSegment->endVertex)
        nodes[i].adjacency.push_back(&nodes[j]);
    }
}

void SkippyGraph::createNodes() {
  for (auto &onSegment : onSegments)
    nodes.push_back({&onSegment, {}});
}

void SkippyGraph::findAllLongestPaths() {
  for (auto &node : nodes)
    findAllPaths(node, {});

  uint maxSize = 0;
  for (auto &path : paths)
    if (path.size() > maxSize)
      maxSize = path.size();

  // erase all smaller paths
  paths.erase(remove_if(paths.begin(), paths.end(),
                        [maxSize](vector<OnSegment *> const &path) {
                          return path.size() < maxSize;
                        }),
              paths.end());
}

void SkippyGraph::findAllPaths(const SkippyNode &node,
                               vector<OnSegment *> path) {
  path.push_back(node.onSegment);
  if (node.adjacency.empty()) {
    paths.push_back(path);
    return;
  }
  for (auto &adjNode : node.adjacency) {
    vector<OnSegment *> tmp(path);
    findAllPaths(*adjNode, tmp);
  }
}

void SkippyGraph::computeOffSegments() {
  offSegments.clear();
  const vector<OnSegment *> &path = paths[pathIdSeclected];
  // Take the origin of any input rays to have the camera position.
  const qglviewer::Vec &cameraCenter = inputRays[0].orig;

  for (uint i = 0; i < path.size() - 1; i++) {
    const PointSequence &lastOnVertex = path[i]->vertices.pointsSeq.back();
    const PointSequence &firstOnVertex =
        path[i + 1]->vertices.pointsSeq.front();
    OffSegment offSegment = {{{}},
                             lastOnVertex.intersectOrder,
                             firstOnVertex.intersectOrder,
                             lastOnVertex.ray.index,
                             firstOnVertex.ray.index};
    double d1 = (cameraCenter - lastOnVertex.pos).norm();
    double d2 = (cameraCenter - firstOnVertex.pos).norm();

    int offRaysRange = (firstOnVertex.ray.index - 1) - lastOnVertex.ray.index;

    for (int rayIndex = lastOnVertex.ray.index + 1;
         rayIndex < firstOnVertex.ray.index; rayIndex++) {
      double offDistanceToCamera = lerp(d1, d2,
                                        (rayIndex - (lastOnVertex.ray.index)) /
                                            double(offRaysRange + 1));
      PointSequence offPoint = {inputRays[rayIndex],
                                inputRays[rayIndex].orig +
                                    inputRays[rayIndex].dir *
                                        offDistanceToCamera,
                                -1.0, -1};
      offSegment.vertices.addPoint(offPoint);
    }
    offSegments.push_back(offSegment);
  }
  computeExtremOffSegments();
}

void SkippyGraph::computeExtremOffSegments() {
  const vector<OnSegment *> &path = paths[pathIdSeclected];
  // Take the origin of any input rays to have the camera position.
  const qglviewer::Vec &cameraCenter = inputRays[0].orig;

  const OnSegment &firstSgement = *path.front();

  if (firstSgement.startVertex > 0) {
    double deltaD = 0;
    // max number of vertices for average change of distance
    const uint MAX_NB_VERTICES = 4;
    // number of vertices for average change of distance
    const uint nbVertices =
        MAX_NB_VERTICES % firstSgement.vertices.pointsSeq.size();

    for (uint i = 0; i < nbVertices - 1; i++) {
      double d1 =
          (cameraCenter - firstSgement.vertices.pointsSeq[i].pos).norm();
      double d2 =
          (cameraCenter - firstSgement.vertices.pointsSeq[i + 1].pos).norm();
      deltaD += d1 - d2;
    }
    deltaD /= (double)nbVertices;
    double d =
        (cameraCenter - firstSgement.vertices.pointsSeq.front().pos).norm();

    OffSegment offSegment = {
        {{}}, -1, firstSgement.intersectOrder, 0, firstSgement.startVertex};

    for (int q = 0; q < firstSgement.startVertex; q++) {
      qglviewer::Vec offPoint =
          cameraCenter +
          (d + (firstSgement.startVertex - q) * deltaD) * inputRays[q].dir;
      PointSequence offSeq = {inputRays[q], offPoint, -1.0, -1};
      offSegment.vertices.addPoint(offSeq);
    }
    offSegments.push_back(offSegment);
  }
}
