#include "../headers/main_model.h"

namespace s21 {

Scene QtSceneDrawer::ReadFile(const std::string &path) {
  try {
    return this->file_reader_.ReadFile(path);
  } catch (...) {
    throw std::logic_error("something wrong reading file");
  }
};

Point3D QtSceneDrawer::LoadScene(const Scene &scene) {
  Point3D move_shift;
  //  try {
  this->norm_params_.clearParams();
  move_shift = this->norm_params_.NormalaizeParametersScene(scene);
  // } catch (...) {return false; } return true;
  return move_shift;
};

void QtSceneDrawer::SaveSetting() { this->settings_.SaveSettings(); };

int QtSceneDrawer::LoadSetting() { return this->settings_.LoadSettings(); };

BaseSettings &QtSceneDrawer::GetSettings() { return this->settings_; };

const BaseNormalizationParameter &QtSceneDrawer::GetNormalaizeParamsScene() {
  return this->norm_params_;
};
}  // namespace s21
