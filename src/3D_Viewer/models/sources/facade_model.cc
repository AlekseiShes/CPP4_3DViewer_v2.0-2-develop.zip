#include "../headers/main_model.h"

namespace s21 {

FacadeOperations Facade::LoadScene(const std::string &path) {
  try {
    this->scene_.clear();
    this->scene_ = this->sceneDrawer_->ReadFile(path);
    Point3D t = this->sceneDrawer_->LoadScene(this->scene_);
    OrientScene(Orient::Norm_Shift, t);
  } catch (std::exception &e) {
    FacadeOperations res = CreateErrorReport(e.what(), false);
    return res;
  }
  return CreateErrorReport("", true);
};

FacadeOperations Facade::SaveSetting() {
  try {
    this->sceneDrawer_->SaveSetting();
  } catch (std::exception &e) {
    return CreateErrorReport(e.what(), false);
  }
  return CreateErrorReport("", true);
};

FacadeOperations Facade::LoadSetting() {
  std::string loadsetting_warning = "";
  try {
    if (this->sceneDrawer_->LoadSetting() != 0)
      loadsetting_warning = "file \"settings.txt\" was not opened";
  } catch (std::exception &e) {
    return CreateErrorReport(e.what(), false);
  }
  return CreateErrorReport(loadsetting_warning, true);
};

FacadeOperations Facade::SetPath(const std::string &path) {
  try {
    this->sceneDrawer_->GetSettings().SetPath(path);
  } catch (std::exception &e) {
    return CreateErrorReport(e.what(), false);
  }
  return CreateErrorReport("", true);
};
FacadeOperations Facade::SetSetting(const std::string &method_name,
                                    const double &value) {
  try {
    this->sceneDrawer_->GetSettings().SetSetting(method_name, value);
  } catch (std::exception &e) {
    return CreateErrorReport(e.what(), false);
  }
  return CreateErrorReport("", true);
};

FacadeOperations Facade::SetColor(const std::string &method_name,
                                  const Point3D &color) {
  try {
    this->sceneDrawer_->GetSettings().SetColor(method_name, color);
  } catch (std::exception &e) {
    return CreateErrorReport(e.what(), false);
  }
  return CreateErrorReport("", true);
};

FacadeOperations Facade::OrientScene(const Orient &method,
                                     const Point3D &params) {
  try {
    switch (method) {
      case Orient::Reset: {
        Move_ = TransformMatrixBilder::CreateMoveMatrix(Point3D(0, 0, 0));
        Rotate_ = TransformMatrixBilder::CreateRotationMatrix(Point3D(0, 0, 0));
        Scale_ = TransformMatrixBilder::CreateScaleMatrix(Point3D(1, 1, 1));
        Move_Shift_ = TransformMatrixBilder::CreateMoveMatrix(Point3D(0, 0, 0));
        break;
      }
      case Orient::Move: {
        Move_ = TransformMatrixBilder::CreateMoveMatrix(params);
        break;
      }
      case Orient::Rotation: {
        Rotate_ = TransformMatrixBilder::CreateRotationMatrix(params);
        break;
      }
      case Orient::Scale: {
        Scale_ = TransformMatrixBilder::CreateScaleMatrix(params);
        break;
      }
      case Orient::Norm_Shift: {
        Move_Shift_ = TransformMatrixBilder::CreateMoveMatrix(params);
        break;
      }
      default: {
        break;
      }  // return CreateErrorReport(e.what(), false);
    }
    Modify_ = Move_ * Rotate_ * Scale_ * Move_Shift_;
    if (method != Orient::Reset) scene_.TransformFigures(Modify_);
  } catch (std::exception &e) {
    return CreateErrorReport(e.what(), false);
  }
  return CreateErrorReport("", true);
};

const Scene &Facade::GetScene() { return this->scene_; };

const TransformMatrix &Facade::GetTransformMatrix() { return this->Modify_; };

std::string Facade::GetPath() {
  return this->sceneDrawer_->GetSettings().GetPath();
};

double Facade::GetSetting(const std::string &method_name) {
  return this->sceneDrawer_->GetSettings().GetSetting(method_name);
};

float Facade::GetColor(const std::string &method_name) {
  return this->sceneDrawer_->GetSettings().GetColor(method_name);
};

std::any Facade::GetFacets() {
  return this->sceneDrawer_->GetNormalaizeParamsScene().GetFacets();
};

std::any Facade::GetVertexes() {
  return this->sceneDrawer_->GetNormalaizeParamsScene().GetVertexes();
};

std::any Facade::GetMaxCoordinate() {
  return this->sceneDrawer_->GetNormalaizeParamsScene().GetMaxCoordinate();
};

FacadeOperations Facade::CreateErrorReport(const std::string &error,
                                           bool is_succes) {
  FacadeOperations result(error, is_succes);
  return result;
};
}  // namespace s21
