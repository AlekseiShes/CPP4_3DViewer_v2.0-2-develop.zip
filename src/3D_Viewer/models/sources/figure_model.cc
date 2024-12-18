#include "../headers/main_model.h"

namespace s21 {
const std::vector<Vertex> &Figure::GetVertexes() const {
  return this->vertexes_;
};

const std::vector<double> &Figure::GetLocation() const {
  return this->location_;
}

const std::vector<Edge> &Figure::GetEdges() const { return this->edges_; };

void Figure::PushEdge(const std::vector<int> &other) {
  Edge edge;
  edge.SetEdge(other);
  this->edges_.push_back(edge);
};

void Figure::Transform(const TransformMatrix &modify) {
  for (auto &vertex : vertexes_) {
    vertex.Transform(modify);
  }
}

void Figure::TransformLoc(const TransformMatrix &modify) {
  if (vertexes_.size() * 3 != location_.size())
    location_.resize(vertexes_.size() * 3);
  if (modify.Matrix.getCols() > 3 && modify.Matrix.getRows() > 3) {
    size_t i = 0;
    for (size_t v = 0; v < vertexes_.size(); ++v)
      for (i = 0; i < 3; ++i)
        location_[v * 3 + i] =
            modify.Matrix(i, 0) * vertexes_[v].GetPosition()[0] +
            modify.Matrix(i, 1) * vertexes_[v].GetPosition()[1] +
            modify.Matrix(i, 2) * vertexes_[v].GetPosition()[2] +
            modify.Matrix(i, 3);
  }
};

void Figure::PushVertex(const Point3D &point) {
  Vertex vertex;
  vertex.SetPosition(point);
  vertex.SetPositionOnStart(point);
  this->vertexes_.push_back(vertex);
};

void Figure::clearFigure() {
  this->edges_.clear();
  this->vertexes_.clear();
  this->location_.clear();
};

Figure::~Figure() { this->clearFigure(); };
}  // namespace s21
