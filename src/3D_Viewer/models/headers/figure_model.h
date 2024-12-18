#ifndef CPP4_3DVIEWER_FIGURE_MODEL_H
#define CPP4_3DVIEWER_FIGURE_MODEL_H

#include <stdint.h>

#include <vector>

#include "edge_model.h"
#include "vertex_model.h"

namespace s21 {
class Figure /*: public SceneObject*/ {
 public:
  Figure() = default;
  ~Figure();
  const std::vector<Vertex> &GetVertexes() const;
  const std::vector<double> &GetLocation() const;
  const std::vector<Edge> &GetEdges() const;
  void PushVertex(const Point3D &point);
  void PushEdge(const std::vector<int> &other);
  void Transform(const TransformMatrix &modify);
  void TransformLoc(const TransformMatrix &modify);
  void clearFigure();

 private:
  std::vector<Vertex> vertexes_{};
  std::vector<double> location_{};
  std::vector<Edge> edges_{};
};
};  // namespace s21
#endif
