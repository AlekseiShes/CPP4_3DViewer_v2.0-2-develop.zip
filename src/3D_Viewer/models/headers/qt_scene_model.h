#ifndef CPP4_3DVIEWER_QT_SCENE_DRAWER_MODEL_H
#define CPP4_3DVIEWER_QT_SCENE_DRAWER_MODEL_H

#if !defined __APPLE__
#include <GL/glu.h>
#endif

#include "base_scene_model.h"
#include "qt_file_reader_model.h"
#include "qt_normalize_model.h"
#include "qt_settings_model.h"

namespace s21 {
class QtSceneDrawer : public BaseScene {
 public:
  Scene ReadFile(const std::string &path) override;
  Point3D LoadScene(const Scene &scene) override;
  virtual void SaveSetting() override;
  virtual int LoadSetting() override;
  BaseSettings &GetSettings() override;
  const BaseNormalizationParameter &GetNormalaizeParamsScene() override;

 private:
  QtSettings settings_;
  QtNormalizationParameters norm_params_;
  QtFileReader file_reader_;
};
};  // namespace s21
#endif
