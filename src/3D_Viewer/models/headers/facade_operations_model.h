#ifndef CPP4_3DVIEWER_FACADE_OPERATIONS_RESULT_MODEL_H
#define CPP4_3DVIEWER_FACADE_OPERATIONS_RESULT_MODEL_H
#include <string>

namespace s21 {
class FacadeOperations {
 public:
  FacadeOperations() = default;
  FacadeOperations(const std::string &message, bool status)
      : errorMessage_(message), isSucces_(status) {}
  const std::string &GetErrorMessage();
  bool IsSucces();

 private:
  std::string errorMessage_;
  bool isSucces_;
};
};  // namespace s21
#endif
