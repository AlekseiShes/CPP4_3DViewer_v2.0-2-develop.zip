#include "../headers/main_model.h"

namespace s21 {
const std::string &FacadeOperations::GetErrorMessage() {
  return this->errorMessage_;
};

bool FacadeOperations::IsSucces() { return this->isSucces_; };
}  // namespace s21