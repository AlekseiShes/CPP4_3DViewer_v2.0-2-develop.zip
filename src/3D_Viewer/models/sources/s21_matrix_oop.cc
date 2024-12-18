#include "../headers/s21_matrix_oop.h"

namespace s21 {

/* Destructor */
S21Matrix::~S21Matrix() { Delete(); }

void S21Matrix::Delete() {
  if (this->matrix_) {
    for (int i = 0; i < this->rows_; ++i) delete[] this->matrix_[i];
    delete[] this->matrix_;
  }
  Reset();
}

void S21Matrix::Reset() {
  this->cols_ = 0;
  this->rows_ = 0;
  this->matrix_ = nullptr;
}

/* Constructor (Empty)*/
S21Matrix::S21Matrix() : rows_(0), cols_(0), matrix_(nullptr) { /*Reset();*/
}

/* Constructor (With arguments)*/
S21Matrix::S21Matrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0)
    throw std::logic_error(
        "Row and column count values must be natural numbers!");
  else
    this->Create(rows, cols);
}

void S21Matrix::Create(int rows, int cols) {
  if (rows > 0 && cols > 0) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->matrix_ = new double *[this->rows_]();
    for (int i = 0; i < this->rows_; ++i) {
      this->matrix_[i] = new double[this->cols_]();
      for (int j = 0; j < this->cols_; ++j) matrix_[i][j] = 0;
    }
  }
  // else {
  //   throw std::logic_error(
  //       "Row and column count values must be natural numbers!");
  // }
}

/* copy-Constructor */
S21Matrix::S21Matrix(const S21Matrix &other) {
  if (this != &other) {
    Reset();
    this->Create(other.rows_, other.cols_);
    this->Linear_Arithmetic(1, {&other});
  }
}

/* Arithmetic funtions */
void S21Matrix::Linear_Arithmetic(double num, const S21Matrix *other) {
  if (other && !IsEqualSize(*other)) {
    throw std::out_of_range("Matrices of different sizes!");
  }
  bool otherIsOk = other && other->matrix_;
  if (this->IsCorrect()) {
    for (int i = 0; i < this->rows_; ++i) {
      for (int j = 0; j < this->cols_; ++j) {
        if (otherIsOk)
          this->matrix_[i][j] += num * other->matrix_[i][j];
        else
          this->matrix_[i][j] *= num;
      }
    }
  }
}

bool S21Matrix::IsEqualSize(const S21Matrix &other) {
  return this->cols_ == other.cols_ && this->rows_ == other.rows_;
}

bool S21Matrix::IsCorrect() const {
  return this->cols_ > 0 && this->rows_ > 0 && this->matrix_;
}

// Copy assignment operator
S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this != &other) {
    this->Delete();
    this->Create(other.rows_, other.cols_);
    this->Linear_Arithmetic(1, {&other});
  }
  return *this;
}

/* move-Constructor */
S21Matrix::S21Matrix(S21Matrix &&other) noexcept {
  if (other.IsCorrect() && this != &other) {
    this->Delete();
    moveM(other);
    other.Reset();
  }
}

void S21Matrix::moveM(const S21Matrix &other) {
  if (this != &other) {
    this->cols_ = other.cols_;
    this->rows_ = other.rows_;
    this->matrix_ = other.matrix_;
  }
}

// Move assignment operator
S21Matrix &S21Matrix::operator=(S21Matrix &&other) noexcept {
  if (other.IsCorrect() && this != &other) {
    this->Delete();
    moveM(other);
    other.Reset();
  }
  return *this;
}

// Проверяет матрицы на равенство между собой
bool S21Matrix::EqMatrix(const S21Matrix &other) {
  bool isEqual = this == &other;
  if (!isEqual) {
    isEqual = IsEqualSize(other);
    for (int i = 0; i < this->rows_ && isEqual; ++i)
      for (int j = 0; j < this->cols_ && isEqual; ++j)
        isEqual = (fabs(this->matrix_[i][j] - other.matrix_[i][j]) < PRECISION);
  }
  return isEqual;
}

//Прибавляет вторую матрицы к текущей
void S21Matrix::SumMatrix(const S21Matrix &other) {
  Linear_Arithmetic(1, {&other});
}

// Вычитает из текущей матрицы другую
void S21Matrix::SubMatrix(const S21Matrix &other) {
  Linear_Arithmetic(-1, {&other});
}

// Умножает текущую матрицу на число
void S21Matrix::MulNumber(const double num) { Linear_Arithmetic(num, nullptr); }

// Умножает текущую матрицу на вторую
void S21Matrix::MulMatrix(const S21Matrix &other) {
  // if (this->rows_ != other.cols_) { // error
  if (this->cols_ != other.rows_) {  // this 20*5; other 5*1
    throw std::logic_error(
        "amount of columns 1st matrix != amount of rows 2d matrix");
  }
  if (this->IsCorrect() && other.IsCorrect()) {
    S21Matrix temp_result(this->rows_, other.cols_);
    for (int i = 0; i < this->rows_; ++i) {
      for (int j = 0; j < other.cols_; ++j) {
        for (int k = 0; k < this->cols_; k++) {
          temp_result.matrix_[i][j] +=
              this->matrix_[i][k] * other.matrix_[k][j];
        }
      }
    }
    this->setCols(temp_result.cols_);
    this->setRows(temp_result.rows_);
    Linear_Arithmetic(-1, this);
    Linear_Arithmetic(1, &temp_result);
  }
}

void S21Matrix::setCols(int cols) {
  S21Matrix transposed = this->Transpose();
  transposed.setRows(cols);
  S21Matrix resized_matrix = transposed.Transpose();
  this->Delete();
  moveM(resized_matrix);
  resized_matrix.Reset();
}

void S21Matrix::setRows(int rows) {
  if (rows != this->rows_ && this->matrix_) {
    S21Matrix resized_matrix(rows, this->cols_);
    for (int i = 0; i < (rows < this->rows_ ? rows : this->rows_); ++i) {
      for (int j = 0; j < this->cols_; ++j) {
        resized_matrix.matrix_[i][j] = this->matrix_[i][j];
      }
    }
    this->Delete();
    moveM(resized_matrix);
    resized_matrix.Reset();
  }
}

// Создает новую транспонированную матрицу из текущей и возвращает ее
S21Matrix S21Matrix::Transpose() {
  S21Matrix out_matrix;
  if (IsCorrect()) {
    S21Matrix transposed_matrix(cols_, rows_);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        transposed_matrix.matrix_[j][i] = matrix_[i][j];
      }
    }
    out_matrix = transposed_matrix;
  }
  return out_matrix;
}

// Вычисляет матрицу алгебраических дополнений
// текущей матрицы и возвращает ее
S21Matrix S21Matrix::CalcComplements() {
  if (rows_ != cols_) throw std::logic_error("The matrix is not square!");
  S21Matrix result(rows_, cols_);
  if (IsCorrect()) {
    if (this->rows_ == 1) {
      result.matrix_[0][0] = 1;
    } else {
      S21Matrix minor(this->rows_ - 1, this->cols_ - 1);
      for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
          minor.fillMinor(*this, i, j);
          result.matrix_[i][j] += pow(-1, i + j) * minor.Determinant();
        }
      }
    }
  }
  return result;
}

void S21Matrix::fillMinor(const S21Matrix &other, int row, int col) {
  for (int i = 0, r = 0; i < other.rows_; ++i) {
    if (i != row) {
      for (int j = 0, c = 0; j < other.cols_; ++j) {
        if (j != col) {
          matrix_[r][c] = other.matrix_[i][j];
          ++c;
        }
      }
      ++r;
    }
  }
}

// Вычисляет и возвращает определитель текущей матрицы
double S21Matrix::Determinant() {
  if (rows_ != cols_) throw std::logic_error("The matrix is not square!");
  double result = 0;
  if (IsCorrect()) {
    if (this->rows_ == 1) {
      result = matrix_[0][0];
    } else if (this->rows_ == 2) {
      result = matrix_[0][0] * matrix_[1][1] - matrix_[1][0] * matrix_[0][1];
    } else {
      S21Matrix minor(this->rows_ - 1, this->cols_ - 1);
      for (int i = 0; i < this->cols_; ++i) {
        minor.fillMinor(*this, 0, i);
        result += matrix_[0][i] * pow(-1, i) * minor.Determinant();
      }
    }
  }
  return result;
}

// Вычисляет и возвращает обратную матрицу
S21Matrix S21Matrix::InverseMatrix() {
  double det = Determinant();
  if (fabs(det) < PRECISION)
    throw std::invalid_argument("The determinant is 0!");
  // return S21Matrix(((*this).CalcComplements().Transpose())* (1 / det));
  S21Matrix temp = (((*this).CalcComplements()).Transpose()) * (1 / det);
  // S21Matrix temp = (*this).CalcComplements();
  // temp = temp.Transpose();
  // temp *= (1 / det);
  this->Delete();
  moveM(temp);
  temp.Reset();
  return *this;
}

/* Operator funtions */

S21Matrix S21Matrix::operator+(const S21Matrix &other) {
  S21Matrix new_matrix(*this);
  new_matrix.SumMatrix(other);
  return new_matrix;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) {
  S21Matrix new_matrix(*this);
  new_matrix.SubMatrix(other);
  return new_matrix;
}

// template <>
S21Matrix S21Matrix::operator*(const S21Matrix &other) const {
  S21Matrix new_matrix(*this);
  new_matrix.MulMatrix(other);
  return new_matrix;
}

// template <typename Point>
//   Point S21Matrix::operator*(const Point &other) {
//   Point Pnt;
//   if (this->cols_ < other.dimension() ||
//       this->rows_ < Pnt.dimension())
//     throw std::logic_error(
//       "It is not possible to multiply a matrix (r="
//       + std::to_string(this->rows_)
//       + "; c=" + std::to_string(this->cols_)
//       + ") by a column (r=" + std::to_string(other.dimension()) +
//       ") and get a column (r=" + std::to_string(Pnt.dimension()) + ")");
//   if (this->IsCorrect())
//     for(size_t i = 0; i < Pnt.dimension(); ++i)
//       for (size_t j = 0; j < this->cols_; ++j)
//         Pnt[i] += this->matrix_[i][j] *
//           ((j < other.dimension()) ? other[j] : 1);
//   return Pnt;
// }

S21Matrix S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator/(const S21Matrix &other) {
  S21Matrix new_matrix(*this);
  S21Matrix inversed(other);
  inversed = inversed.InverseMatrix();
  new_matrix.MulMatrix(inversed);
  return new_matrix;
}

S21Matrix S21Matrix::operator/=(const S21Matrix &other) {
  S21Matrix inversed(other);
  inversed = inversed.InverseMatrix();
  MulMatrix(inversed);
  return *this;
}

S21Matrix S21Matrix::operator*(const double num) {
  S21Matrix new_matrix(*this);
  new_matrix.MulNumber(num);
  return new_matrix;
}

S21Matrix /*S21Matrix::*/ operator*(const double &num, const S21Matrix &other) {
  S21Matrix new_matrix(other);
  new_matrix.MulNumber(num);
  return new_matrix;
}

bool S21Matrix::operator==(const S21Matrix &other) { return EqMatrix(other); }

S21Matrix S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

double &S21Matrix::operator()(int i, int j) {
  CheckIndexes(i, j);
  return matrix_[i][j];
}

/* Check functions */
void S21Matrix::CheckInRange(int i, int max, const char *name) const {
  if (!(0 <= i && i < max)) {
    char msg[250] = {'\0'};
    sprintf(msg, "%10s = %d is out of range (0 .. %d)\n", name, i, max);
    throw std::out_of_range(msg);
  }
}

void S21Matrix::CheckIndexes(int row, int col) const {
  CheckInRange(row, this->rows_, "Row");
  CheckInRange(col, this->cols_, "Col");
  if (!this->matrix_) throw std::logic_error("Matrix is not exist!\n");
}

/* Get Set functions */
void S21Matrix::setValue(int row, int col, double val) {
  CheckIndexes(row, col);
  matrix_[row][col] = val;
}

double S21Matrix::getValue(int row, int col) const noexcept {
  return (*this)(row, col);
}

int S21Matrix::getRows() const noexcept {
  return this->matrix_ ? this->rows_ : 0;
}

int S21Matrix::getCols() const noexcept {
  return this->matrix_ ? this->cols_ : 0;
}

// Делит текущую матрицу на число
void S21Matrix::DivNumber(const double num) {
  if (fabs(num) < PRECISION) {
    char msg[250] = {'\0'};
    sprintf(msg,
            "The division by zero or the |%.2e| is less then PRECISION=%.2e \n",
            num, PRECISION);
    throw std::invalid_argument(msg);
  }
  Linear_Arithmetic(1 / num, nullptr);
}

S21Matrix S21Matrix::operator/(const double num) {
  S21Matrix new_matrix(*this);
  new_matrix.DivNumber(num);
  return new_matrix;
}

S21Matrix S21Matrix::operator/=(const double num) {
  DivNumber(num);
  return *this;
}

S21Matrix /* S21Matrix::*/ operator/(const double &num,
                                     const S21Matrix &other) {
  S21Matrix inversed(other);
  inversed = inversed.InverseMatrix();
  inversed.MulNumber(num);
  return inversed;
}

std::string S21Matrix::to_string(unsigned precision) const {
  std::ostringstream oss;
  oss << " rows == " << std::to_string(this->rows_)
      << "; cols == " << std::to_string(this->cols_) << "; matrix == ";
  if (matrix_ == nullptr) {
    oss << "nullptr";
    return oss.str();
  }
  oss << "\n{";  // далее Перебор элементов матрицы
  for (int i = 0; i < rows_; ++i) {
    oss << "{";
    for (int j = 0; j < cols_; ++j) {  // Добавляем элемент матрицы
      oss << std::fixed << std::setprecision(precision) << getValue(i, j);
      if (j < cols_ - 1) oss << ",\t";  // разделитель между элементами
    }
    oss << "}";
    if (i < rows_ - 1) oss << ",\n ";  // новую строку
  }
  oss << "}\n";
  return oss.str();
};

const double &S21Matrix::operator()(int i, int j) const {
  CheckIndexes(i, j);  // Убедитесь, что индексы в допустимых пределах
  return matrix_[i][j];
}

}  // namespace s21
