#ifndef CPP4_3DVIEWER_VERTEX_MODEL_H_
#define CPP4_3DVIEWER_VERTEX_MODEL_H_

#include <vector>

#include "3dpoint_model.h"
#include "transformmatrix_model.h"

namespace s21 {

// class SceneObject {
//  public:
//   virtual ~SceneObject() = default;  // Виртуальный деструктор
//   virtual void Transform(const TransformMatrix &modify);
// };

class Vertex /*: public SceneObject*/ {
 public:
  const Point3D &GetPosition() const;
  inline void Transform(const TransformMatrix &modify) { /*override*/
    position_ = modify.TransformPoint(position_start_);
  };
  void SetPosition(const Point3D &point);
  void SetPositionOnStart(const Point3D &point);

 private:
  Point3D position_;
  Point3D position_start_;  // start position
};
};  // namespace s21
#endif
