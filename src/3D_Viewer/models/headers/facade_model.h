#ifndef CPP4_3DVIEWER_FACADE_MODEL_H
#define CPP4_3DVIEWER_FACADE_MODEL_H

#include <memory>

#include "../headers/transformmatrix_model.h"
#include "decorator_scene_model.h"
#include "facade_operations_model.h"

namespace s21 {
class Facade {
 public:
  Facade(DecoratorScene *other_scene_drawer)
      : sceneDrawer_(other_scene_drawer) {}

  FacadeOperations LoadScene(const std::string &path);
  FacadeOperations SaveSetting();
  FacadeOperations LoadSetting();
  FacadeOperations SetPath(const std::string &path);
  FacadeOperations SetSetting(const std::string &method_name,
                              const double &value);
  FacadeOperations SetColor(const std::string &method_name,
                            const Point3D &color);
  FacadeOperations OrientScene(const Orient &method, const Point3D &params);
  std::string GetPath();
  double GetSetting(const std::string &method_name);
  float GetColor(const std::string &method_name);
  std::any GetFacets();
  std::any GetVertexes();
  std::any GetMaxCoordinate();
  const Scene &GetScene();                      // for test
  const TransformMatrix &GetTransformMatrix();  // for test

 private:
  FacadeOperations CreateErrorReport(const std::string &error, bool is_succes);
  Scene scene_;
  DecoratorScene *sceneDrawer_;
  TransformMatrix Modify_;
  TransformMatrix Move_, Move_Shift_, Rotate_, Scale_;
};
};  // namespace s21
#endif