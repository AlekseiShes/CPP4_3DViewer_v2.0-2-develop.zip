#ifndef CPP4_3DVIEWER_SCENE_MODEL_H
#define CPP4_3DVIEWER_SCENE_MODEL_H
#include "figure_model.h"

namespace s21 {
class Scene {
 public:
  const Figure &GetFigure() const;
  Figure &GetFigure();
  void TransformFigures(const TransformMatrix &modify);
  void clear();

 private:
  Figure figure_;
};
};  // namespace s21
#endif
