#ifndef CPP4_3DVIEWER_3DPOINT_MODEL_H
#define CPP4_3DVIEWER_3DPOINT_MODEL_H
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace s21 {

class Point3D {
 public:
  double x_;
  double y_;
  double z_;
  virtual size_t dimension() const { return 3; }  // Виртуальный метод

  // Point3D() = default;
  explicit Point3D(double x = 0, double y = 0, double z = 0)
      : x_(x), y_(y), z_(z) {}
  // Конструктор копирования
  Point3D(const Point3D &other) : x_(other.x_), y_(other.y_), z_(other.z_) {}
  // Конструктор перемещения
  Point3D(Point3D &&other) noexcept : x_(other.x_), y_(other.y_), z_(other.z_) {
    other.x_ = other.y_ = other.z_ = 0;
  }

  // Оператор индексации для чтения
  virtual double operator[](size_t index) const {
    if (index >= dimension()) throw std::out_of_range("Index out of range");
    return index == 0 ? x_ : (index == 1 ? y_ : z_);
  }
  // Оператор индексации для записи
  virtual double &operator[](size_t index) {
    if (index >= dimension()) throw std::out_of_range("Index out of range");
    return index == 0 ? x_ : (index == 1 ? y_ : z_);
  }

  // Оператор присвоения копированием
  Point3D &operator=(const Point3D &other) {
    if (this != &other) {  // Проверка на самоприсваивание
      for (size_t i = 0; i < this->dimension() && i < other.dimension(); i++)
        this->operator[](i) = other[i];
    }
    return *this;
  }

  // Оператор присвоения перемещением
  Point3D &operator=(Point3D &&other) noexcept {
    if (this != &other) {  // Проверка на самоприсваивание
      for (size_t i = 0; i < this->dimension() && i < other.dimension(); i++) {
        this->operator[](i) = other[i];
        other[i] = 0;
      }
    }
    return *this;
  }

  // Оператор вычитания
  template <typename Point>
  Point operator-(const Point &other) const noexcept {
    // Point3D operator-(const Point3D &other) const noexcept {
    Point res;
    for (size_t i = 0; i < this->dimension() && i < other.dimension(); i++)
      res[i] = this->operator[](i) - other[i];
    return res;
  }

  // Оператор сложения
  template <typename Point>
  Point operator+(const Point &other) const noexcept {
    Point res;
    for (size_t i = 0; i < this->dimension() && i < other.dimension(); i++)
      res[i] = this->operator[](i) + other[i];
    return res;
  }
  // умножение на число
  Point3D &operator*(const double &num) noexcept {
    for (size_t i = 0; i < this->dimension(); ++i) this->operator[](i) *= num;
    return *this;
  }

  // Оператор сравнения
  template <typename Point>
  bool operator==(const Point &other) const noexcept {
    if (this->dimension() != other.dimension()) return false;
    // throw std::logic_error(
    //   "Unable to compare this->dimension()="
    //   + std::to_string(this->dimension())
    //   + " and column other.dimension()="
    //   + std::to_string(other.dimension()));
    for (size_t i = 0; i < this->dimension() && i < other.dimension(); i++)
      if (this->operator[](i) != other[i]) return false;
    return true;
  }

  template <typename Point>
  friend double length(const Point &point) noexcept {
    double res = 0;
    for (size_t i = 0; i < point.dimension(); i++) res += pow(point[i], 2);
    return sqrt(res);
  }

  virtual std::string to_string(unsigned precision = 6) const {
    std::ostringstream oss;
    oss << "(";
    for (size_t i = 0; i < dimension(); i++) {
      oss << std::fixed << std::setprecision(precision) << this->operator[](i);
      if (i + 1 < dimension()) oss << "; ";
    }
    oss << ")";
    return oss.str();
  }

  virtual ~Point3D() = default;
};

class Point4D : public Point3D {
 public:
  double w_;
  size_t dimension() const override { return 4; }  // Переопределяем размерность

  explicit Point4D(double x = 0, double y = 0, double z = 0, double w = 1)
      : Point3D(x, y, z), w_(w) {}

  // // Конструктор копирования
  // Point4D (const Point4D &other){
  //     if (this != &other) {
  //     Point3D::operator=(other);  // Копируем базовый класс
  //     w_ = other.w_;
  //   } //return *this;
  // }

  // Конструктор копирования
  Point4D(const Point4D &other)
      : Point3D(other),  // Инициализация базового класса
        w_(other.w_){};  // Инициализация собственного члена класса

  // Конструктор перемещения
  Point4D(Point4D &&other) noexcept = default;

  // Оператор присвоения копированием
  // Point4D &operator=(const Point4D &other) = default;
  Point4D &operator=(const Point4D &other) {
    // if (this != &other) {
    //   Point3D::operator=(other);  // Копируем базовый класс
    //   w_ = other.w_;
    // }
    this->x_ = other.x_;
    this->w_ = other.w_;
    this->y_ = other.y_;
    this->z_ = other.z_;
    return *this;
  }

  // Оператор присвоения перемещением
  Point4D &operator=(Point4D &&other) noexcept = default;

  double operator[](size_t index) const override {
    if (index + 1 == dimension()) return w_;
    return Point3D::operator[](index);
  }

  double &operator[](size_t index) override {
    if (index + 1 == dimension()) return w_;
    return Point3D::operator[](index);
  }
};

};  // namespace s21
#endif
