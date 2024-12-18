#ifndef CPP4_3DVIEWER_BASE_FILE_READER_MODEL_H
#define CPP4_3DVIEWER_BASE_FILE_READER_MODEL_H

#include <string>
#include <vector>

#include "scene_model.h"

namespace s21 {
class BaseFileReader {
 public:
  virtual Scene ReadFile(const std::string &path) = 0;
};
};  // namespace s21
#endif
