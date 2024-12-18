#ifndef CPP4_3DVIEWER_QT_FILE_READER_MODEL_H
#define CPP4_3DVIEWER_QT_FILE_READER_MODEL_H

#include <stdint.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "base_file_reader_model.h"
#include "scene_model.h"
namespace s21 {

double myconv(std::string expr, char dec);

class QtFileReader : public BaseFileReader {
 public:
  Scene ReadFile(const std::string &path) override;

 private:
  bool LineParser(const std::string &line);
  bool VertexParser(std::vector<std::string> &line);
  bool FasetParser(std::vector<std::string> &line);
  double StrToDouble(const std::string &line, size_t &hashtag_exist);
  std::vector<std::string> SplitStringByAnyDelimiter(
      const std::string &str, const std::string &delimiters);

 private:
  Scene scene_;
};
};  // namespace s21
#endif
