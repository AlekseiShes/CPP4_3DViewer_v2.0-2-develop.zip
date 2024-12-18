#ifndef CPP4_3DVIEWER_EDGE_MODEL_H
#define CPP4_3DVIEWER_EDGE_MODEL_H

#include <stdint.h>

#include <vector>

#include "vertex_model.h"

namespace s21 {
class Edge {
 public:
  Edge() = default;
  ~Edge();
  const std::vector<int> &GetEdge() const;
  void SetEdge(const std::vector<int> &edge);

 private:
  std::vector<int> edge_{};
};
};  // namespace s21
#endif
