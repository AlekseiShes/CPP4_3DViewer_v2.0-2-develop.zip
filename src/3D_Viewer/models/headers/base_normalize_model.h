#ifndef CPP4_3DVIEWER_BASE_NORMALIZATION_PARAMETERS_MODEL_H
#define CPP4_3DVIEWER_BASE_NORMALIZATION_PARAMETERS_MODEL_H
#include <any>

#include "scene_model.h"

namespace s21 {
class BaseNormalizationParameter {
 public:
  virtual Point3D NormalaizeParametersScene(const Scene &scene) = 0;
  virtual Point3D NormalaizeVertexes(const Scene &scene) = 0;
  virtual void NormalaizeFacets(const Scene &scene) = 0;
  virtual std::any GetFacets() const = 0;
  virtual std::any GetVertexes() const = 0;
  virtual std::any GetMaxCoordinate() const = 0;
  virtual void clearParams() = 0;
};
};  // namespace s21
#endif
