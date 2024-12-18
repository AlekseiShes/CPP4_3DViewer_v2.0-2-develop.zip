#ifndef CPP4_3DVIEWER_DECORATOR_SCENE_DRAWER_MODEL_H
#define CPP4_3DVIEWER_DECORATOR_SCENE_DRAWER_MODEL_H

#include "base_scene_model.h"
namespace s21 {

class DecoratorScene : public BaseScene {
 public:
  DecoratorScene(BaseScene *drawer) : sceneDrawer_(drawer) {}

  Scene ReadFile(const std::string &path) override;
  Point3D LoadScene(const Scene &scene) override;
  void SaveSetting() override;
  int LoadSetting() override;
  BaseSettings &GetSettings() override;
  const BaseNormalizationParameter &GetNormalaizeParamsScene() override;

 private:
  BaseScene *sceneDrawer_;
};
};  // namespace s21
#endif
