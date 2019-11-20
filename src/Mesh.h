#ifndef PROJECTMESH_H
#define PROJECTMESH_H

#include "BasicIO.h"
#include "point3.h"

#include <QFileDialog>
#include <QString>
#include <vector>

using namespace std;

struct Vertex {
  point3d p;
  Vertex() {}
  Vertex(double x, double y, double z) : p(x, y, z) {}
  double &operator[](unsigned int c) { return p[c]; }
  double operator[](unsigned int c) const { return p[c]; }
};

struct Triangle {
  unsigned int corners[3];
  unsigned int &operator[](unsigned int c) { return corners[c]; }
  unsigned int operator[](unsigned int c) const { return corners[c]; }
  unsigned int size() const { return 3; }
};

class Mesh {
public:
  vector<Vertex> vertices;
  vector<Triangle> triangles;

  point3d minPoint, maxPoint;

  void clear() {
    vertices.clear();
    triangles.clear();
    minPoint = point3d();
    maxPoint = point3d();
  }

  void draw() {
    glBegin(GL_TRIANGLES);
    for (unsigned int t = 0; t < triangles.size(); ++t) {
      point3d const &p0 = vertices[triangles[t][0]].p;
      point3d const &p1 = vertices[triangles[t][1]].p;
      point3d const &p2 = vertices[triangles[t][2]].p;
      point3d const &n = point3d::cross(p1 - p0, p2 - p0).direction();
      glNormal3f(n[0], n[1], n[2]);
      glVertex3f(p0[0], p0[1], p0[2]);
      glVertex3f(p1[0], p1[1], p1[2]);
      glVertex3f(p2[0], p2[1], p2[2]);
    }
    glEnd();
  }

  void open() {
    bool success = false;
    QString fileName = QFileDialog::getOpenFileName(NULL, "", "");
    if (!fileName.isNull()) { // got a file name
      if (fileName.endsWith(QString(".off")))
        success =
            OFFIO::openTriMesh(fileName.toStdString(), vertices, triangles);
      else if (fileName.endsWith(QString(".obj")))
        success =
            OBJIO::openTriMesh(fileName.toStdString(), vertices, triangles);
      if (success) {
        std::cout << fileName.toStdString() << " was opened successfully"
                  << std::endl;
        point3d bb(FLT_MAX, FLT_MAX, FLT_MAX), BB(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (unsigned int v = 0; v < vertices.size(); ++v) {
          bb = point3d::min(bb, vertices[v]);
          BB = point3d::max(BB, vertices[v]);
        }
        minPoint = bb;
        maxPoint = BB;
      } else
        std::cout << fileName.toStdString() << " could not be opened"
                  << std::endl;
    }
  }

  void save() {
    bool success = false;
    QString fileName = QFileDialog::getOpenFileName(NULL, "", "");
    if (!fileName.isNull()) { // got a file name
      if (fileName.endsWith(QString(".off")))
        success =
            OFFIO::save(fileName.toStdString(), vertices, triangles);
      else if (fileName.endsWith(QString(".obj")))
        success =
            OBJIO::save(fileName.toStdString(), vertices, triangles);
      if (success)
        std::cout << fileName.toStdString() << " was saved" << std::endl;
      else
        std::cout << fileName.toStdString() << " could not be saved"
                  << std::endl;
    }
  }
};

#endif // PROJECTMESH_H
