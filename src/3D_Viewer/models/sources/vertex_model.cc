#include "../headers/vertex_model.h"

#include "../headers/main_model.h"

namespace s21 {

// // Реализация деструктора
// SceneObject::~SceneObject() = default;

// void SceneObject::Transform(const TransformMatrix &modify) {
//   // Можно оставить пустым, если базовый класс не должен ничего делать
// }

const Point3D &s21::Vertex::GetPosition() const { return this->position_; };

// void Vertex::Transform(const TransformMatrix &modify) {
//   position_ = modify.TransformPoint(position_start_);
// };

void Vertex::SetPosition(const Point3D &point) { this->position_ = point; };

void Vertex::SetPositionOnStart(const Point3D &point) {
  this->position_start_ = point;
};

}  // namespace s21
