#ifndef CPP4_3DVIEWER_BASE_SCENE_DRAWER_MODEL_H
#define CPP4_3DVIEWER_BASE_SCENE_DRAWER_MODEL_H
#include <string>

#include "base_normalize_model.h"
#include "base_settings_model.h"
#include "scene_model.h"

namespace s21 {
class BaseScene {
 public:
  virtual Scene ReadFile(const std::string &path) = 0;
  virtual Point3D LoadScene(const Scene &scene) = 0;
  virtual void SaveSetting() = 0;
  virtual int LoadSetting() = 0;
  virtual BaseSettings &GetSettings() = 0;
  virtual const BaseNormalizationParameter &GetNormalaizeParamsScene() = 0;
  virtual ~BaseScene() {}
};
};  // namespace s21
#endif
