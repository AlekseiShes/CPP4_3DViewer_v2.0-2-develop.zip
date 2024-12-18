#ifndef S21_MATRIXPLUS_S21_MATRIX_OOP_H_
#define S21_MATRIXPLUS_S21_MATRIX_OOP_H_

// ****************************************
/*
Number of line - FUNCTION NAME
  6 - Destructor
 23 - Constructor (Empty)
 25 - Constructor (With arguments)
 47 - Copy constructor
 8o - Copy assignment operator =
 91 - Move constructor
1o7 - Move assignment operator =
115 - EqMatrix
127 - SumMatrix
132 - SubMatrix
137 - MulNumber
142 - MulMatrix
189 - Transpose
2o5 - CalcComplements
225 - Determinant
245 - InverseMatrix
      Operators
269 - +
275 - -
281 - *
3o1 - ==
295 - +=
3o8 - -=
313 - *=
324 - ()
352 - getRows getCols

*/
// ****************************************

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

//#include "string.h"
#define PRECISION 1e-07

// #include <fstream>
// inline std::ofstream log_s21("log2.txt", std::ios::app); // Лог сохраняется в
// log.txt #define LOGS21(message) log_s21  << __FUNCTION__ << ": " << message
// << std::endl;

namespace s21 {
class S21Matrix {
 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix &other);
  S21Matrix(S21Matrix &&other) noexcept;
  ~S21Matrix();

  bool EqMatrix(const S21Matrix &other);
  void SumMatrix(const S21Matrix &other);
  void SubMatrix(const S21Matrix &other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix &other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  // service
  void setValue(int row, int col, double val);
  double getValue(int row, int col) const noexcept;
  int getRows() const noexcept;
  int getCols() const noexcept;
  void setRows(int rows);
  void setCols(int cols);

  // operators
  double &operator()(int i, int j);
  const double &operator()(int i, int j) const;  // Для const-объектов
  bool operator==(const S21Matrix &other);
  S21Matrix &operator=(const S21Matrix &other);
  S21Matrix &operator=(S21Matrix &&other) noexcept;
  S21Matrix operator+(const S21Matrix &other);
  S21Matrix operator-(const S21Matrix &other);
  S21Matrix operator*(const double num);
  S21Matrix operator+=(const S21Matrix &other);
  S21Matrix operator-=(const S21Matrix &other);
  S21Matrix operator*=(const double num);
  friend S21Matrix operator*(const double &num, const S21Matrix &other);
  S21Matrix operator*=(const S21Matrix &other);
  S21Matrix operator*(const S21Matrix &other) const;
  // template <typename Point>
  //   Point operator*(const Point &other);

  template <typename Point>
  Point operator*(const Point &other) const {
    Point Pnt;
    if (this->cols_ < (int)other.dimension() ||
        this->rows_ < (int)Pnt.dimension())
      throw std::logic_error("Error");
    // throw std::logic_error("It`s not possible to multiply a matrix (r="
    //   + std::to_string(this->rows_) + "; c=" + std::to_string(this->cols_)
    //   + ") by a column (r=" + std::to_string(other.dimension()) +
    //   ") and get a column (r=" + std::to_string(Pnt.dimension()) + ")");
    if (this->IsCorrect())
      for (int i = 0; i < (int)Pnt.dimension(); ++i) {
        Pnt[i] = 0;
        for (int j = 0; j < this->cols_; ++j)
          Pnt[i] += this->matrix_[i][j] *
                    ((j < (int)other.dimension()) ? other[j] : 1);
      }
    return Pnt;
  }

  // Additional
  S21Matrix operator/(const double num);
  S21Matrix operator/=(const double num);
  S21Matrix operator/=(const S21Matrix &other);
  S21Matrix operator/(const S21Matrix &other);
  friend S21Matrix operator/(const double &num, const S21Matrix &other);
  std::string to_string(unsigned precision = 6) const;

 private:
  int rows_{0}, cols_{0};
  double **matrix_{nullptr};

  void Reset();
  void Delete();
  void Create(int rows, int cols);
  void moveM(const S21Matrix &other);
  bool IsCorrect() const;
  bool IsEqualSize(const S21Matrix &other);
  void CheckInRange(int i, int max, const char *name) const;
  void CheckIndexes(int row, int col) const;
  void Linear_Arithmetic(double num, const S21Matrix *other);
  void fillMinor(const S21Matrix &other, int row, int col);

  // Additional
  void DivNumber(const double num);
};
// S21Matrix operator*(const double &num, const S21Matrix &other);
// Additional
// S21Matrix operator/(const double &num, const S21Matrix &other);
}  // namespace s21

#endif  //  S21_MATRIXPLUS_S21_MATRIX_OOP_H_
