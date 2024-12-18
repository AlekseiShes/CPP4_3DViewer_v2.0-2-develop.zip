#ifndef CPP4_3DVIEWER_QT_NORMALISATION_PARAMETERS_MODEL_H
#define CPP4_3DVIEWER_QT_NORMALISATION_PARAMETERS_MODEL_H

#include <algorithm>
#include <cmath>
#include <vector>

#include "base_normalize_model.h"
#include "scene_model.h"

namespace s21 {
class QtNormalizationParameters : public BaseNormalizationParameter {
 public:
  Point3D NormalaizeParametersScene(const Scene &scene) override;
  Point3D NormalaizeVertexes(const Scene &scene) override;
  void NormalaizeFacets(const Scene &scene) override;
  std::any GetFacets() const override;
  std::any GetVertexes() const override;
  std::any GetMaxCoordinate() const override;
  void clearParams() override;

 private:
  std::vector<double> vertex_normal_;
  std::vector<std::vector<uint32_t>> facet_normal_;
  double max_coordinate_{};
};
};  // namespace s21
#endif
