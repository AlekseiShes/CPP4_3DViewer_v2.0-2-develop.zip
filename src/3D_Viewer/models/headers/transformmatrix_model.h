#ifndef CPP4_3DVIEWER_TRANSFORMMATRIX_MODEL_H_
#define CPP4_3DVIEWER_TRANSFORMMATRIX_MODEL_H_

#include <vector>

#include "3dpoint_model.h"
#include "s21_matrix_oop.h"

namespace s21 {

enum Orient { Reset, Move, Rotation, Scale, Norm_Shift };

// Основной класс для работы с трансформационными матрицами
class TransformMatrix {
 public:
  TransformMatrix();

  // Операция умножения матриц
  TransformMatrix operator*(const TransformMatrix &other);

  // Преобразование точки (применение матрицы к точке)
  Point3D TransformPoint(const Point3D &point) const;

  // Вывод
  // std::string to_string() const;

  // Внутреннее представление матрицы 4x4
  S21Matrix Matrix;

 private:
};

class TransformMatrixBilder {
  // в этом классе static методы, означает, что метод
  //  является статическим членом класса,
  // и его можно вызывать без создания объекта класса
 public:
  // Создание матрицы перемещения
  static TransformMatrix CreateMoveMatrix(const Point3D &move);

  // Создание матрицы поворота
  static TransformMatrix CreateRotationMatrixByAngle(
      const double &ang, const unsigned short &i_of_axes);
  static TransformMatrix CreateRotationMatrix(const Point3D &rota);

  // Создание матрицы масштабирования
  static TransformMatrix CreateScaleMatrix(const Point3D &scal);

 private:
};

};  // namespace s21
#endif
