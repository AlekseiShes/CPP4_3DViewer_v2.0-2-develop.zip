#include "../headers/main_model.h"

namespace s21 {
// bool
Point3D DecoratorScene::LoadScene(const Scene &scene) {
  return this->sceneDrawer_->LoadScene(scene);
};

BaseSettings &DecoratorScene::GetSettings() {
  return this->sceneDrawer_->GetSettings();
};

const BaseNormalizationParameter &DecoratorScene::GetNormalaizeParamsScene() {
  return this->sceneDrawer_->GetNormalaizeParamsScene();
};

Scene DecoratorScene::ReadFile(const std::string &path) {
  return this->sceneDrawer_->ReadFile(path);
};

void DecoratorScene::SaveSetting() { this->sceneDrawer_->SaveSetting(); };

int DecoratorScene::LoadSetting() { return this->sceneDrawer_->LoadSetting(); };
};  // namespace s21
