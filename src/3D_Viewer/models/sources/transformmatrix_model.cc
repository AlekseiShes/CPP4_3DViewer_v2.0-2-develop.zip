#include "../headers/main_model.h"

namespace s21 {

TransformMatrix::TransformMatrix() {
  Matrix = S21Matrix(4, 4);
  Matrix(0, 0) = Matrix(1, 1) = Matrix(2, 2) = Matrix(3, 3) = 1;
};

// Операция умножения матриц
TransformMatrix TransformMatrix::operator*(const TransformMatrix &other) {
  TransformMatrix result;
  result.Matrix = this->Matrix * other.Matrix;  // Умножение матриц S21Matrix
  return result;
}

// Преобразование точки (умножение точки на матрицу)
Point3D TransformMatrix::TransformPoint(const Point3D &point) const {
  return this->Matrix * point;
}

// Метод для создания матрицы перемещения
TransformMatrix TransformMatrixBilder::CreateMoveMatrix(const Point3D &move) {
  TransformMatrix matrix;
  // Заполнение матрицы перемещения (4x4)
  matrix.Matrix(0, 3) = move.x_;
  matrix.Matrix(1, 3) = move.y_;
  matrix.Matrix(2, 3) = move.z_;
  matrix.Matrix(0, 0) = 1;
  matrix.Matrix(1, 1) = 1;
  matrix.Matrix(2, 2) = 1;
  matrix.Matrix(3, 3) = 1;  // Однородные координаты
  return matrix;
}

TransformMatrix TransformMatrixBilder::CreateRotationMatrixByAngle(
    const double &ang, const unsigned short &i_of_axes) {
  TransformMatrix matrix;
  if (i_of_axes > 2) return matrix;
  // { // x: i=0; y: i=1; z: i=2.
  //   // for (int k = 0; k < 3; ++k)
  //   //     matrix.Matrix(k, k) = 1;
  //   return matrix; // return Identity matrix
  // }
  for (int r = 0; r < matrix.Matrix.getRows(); r++) {
    for (int c = 0; c < matrix.Matrix.getCols(); c++) {
      if (r == c) {
        matrix.Matrix(r, c) =
            (r == i_of_axes || r == 3) ? 1 : cos(ang * M_PI / 180);
      } else {
        matrix.Matrix(r, c) = sin(ang * M_PI / 180);
        if (r == (i_of_axes + 1) % 3) matrix.Matrix(r, c) *= (-1);
        if (r == i_of_axes || c == i_of_axes || r == 3 || c == 3)
          matrix.Matrix(r, c) = 0;
      }
    }
  }
  return matrix;
}

// Метод для создания матрицы поворота
TransformMatrix TransformMatrixBilder::CreateRotationMatrix(
    const Point3D &rota) {
  return CreateRotationMatrixByAngle(rota.x_, 0) *
         CreateRotationMatrixByAngle(rota.y_, 1) *
         CreateRotationMatrixByAngle(rota.z_, 2);
}

// Метод для создания матрицы масштабирования
TransformMatrix TransformMatrixBilder::CreateScaleMatrix(const Point3D &scal) {
  TransformMatrix matrix;
  matrix.Matrix(0, 0) = scal.x_;
  matrix.Matrix(1, 1) = scal.y_;
  matrix.Matrix(2, 2) = scal.z_;
  matrix.Matrix(3, 3) = 1;  // Однородные координаты
  return matrix;
}
}  // namespace s21
