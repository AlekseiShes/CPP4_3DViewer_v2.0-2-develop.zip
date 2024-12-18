#include "../headers/main_model.h"

inline bool isnumerically(const char c) {
  return std::isdigit(c) || c == '-' || c == '+';
}

namespace s21 {

double getunsignnum_uptoStop(char **p) {
  double num = 0;
  for (; p && *p && (**p <= '9') && (**p >= '0'); (*p)++)
    num = num * 10 + (**p - '0');
  return num;
}

double myconv(std::string expr, char dec) {
  int sign = 0;
  if (expr[0] == '-') sign = -1;
  if (expr[0] == '+') sign = 1;
  char *p = &expr[(int)abs(sign)];
  double num = getunsignnum_uptoStop(&p);
  if (p && (*p == dec)) {
    p++;
    char *s = p;
    double frac = getunsignnum_uptoStop(&p);
    num += frac * pow(0.1, p - s);
  }
  return num * (sign == 0 ? 1 : sign);
};

Scene QtFileReader::ReadFile(const std::string &path) {
  std::filesystem::path filePath(path);
  if (!std::filesystem::exists(filePath))
    throw std::logic_error("file doesn't exist");
  if (filePath.extension().string() != ".obj")
    throw std::logic_error("file doesn't obj");
  this->scene_.clear();
  std::ifstream file;
  file.open(path);
  if (!file.is_open()) throw std::logic_error("file hasn't opened");
  try {
    auto tmp = std::string::npos;
    std::string str_tmp;
    while (std::getline(file, str_tmp)) {
      // далее обрезаем строку до символа начала комментарий #
      if ((tmp = str_tmp.find_first_of('#')) == std::string::npos)
        LineParser(str_tmp);
      else
        LineParser(str_tmp.substr(0, tmp));
    }
  } catch (const std::exception &e) {
    file.close();
    throw;
  }
  file.close();
  return this->scene_;
};

bool QtFileReader::LineParser(const std::string &line) {
  bool status = false;
  std::vector<std::string> str = SplitStringByAnyDelimiter(line, " \t");
  std::string firstWord;
  if (str.size() > 0) firstWord = str.begin()->data();
  if (firstWord == "v") {
    status = VertexParser(str);
  } else if (firstWord == "f") {
    status = FasetParser(str);
  }  // else if(firstWord == "vt")...
  return status;
};

bool QtFileReader::VertexParser(std::vector<std::string> &line) {
  std::vector<double> point3D;
  size_t hashtag_exist = 0;
  for (std::string str : line) {
    if (str.size() == 0 || str[0] == '#' || hashtag_exist != 0) {
      break;
    } else if (str.size() > 0 && isnumerically(str[0])) {
      point3D.push_back(StrToDouble(str, hashtag_exist));
    }
  }
  if (point3D.size() < 3)
    throw std::logic_error("vertex parser error: point3D < 3");
  this->scene_.GetFigure().PushVertex(
      Point3D(point3D[0], point3D[1], point3D[2]));
  return true;
};

bool QtFileReader::FasetParser(std::vector<std::string> &line) {
  std::vector<int> fasets;  // полагаем что строка уже обрезана по hashtag
  for (std::string str : line) {
    std::vector<std::string> sector = SplitStringByAnyDelimiter(str, "/");
    for (size_t i = 0; i < sector.size(); ++i) {
      const std::string &subs = sector[i];
      if (!subs.empty() && isnumerically(subs[0])) {
        if (i == 0) fasets.push_back(std::round(std::stod(subs)));
        // if (i == 1) // для получения фасетов текстур fasetsVT,
        // if (i == 2) // для получения фасетов нормалей fasetsVN
      }
      break;  // т.к. не прописано для i=1, 2
    }
  }
  if (fasets.size() > 0) this->scene_.GetFigure().PushEdge(fasets);
  return true;
};

double QtFileReader::StrToDouble(const std::string &line,
                                 size_t &hashtag_exist) {
  std::string str;
  size_t i;
  for (i = 0; i < line.length(); i++) {
    if (std::string("# \t\r\n").find(line[i]) != std::string::npos) {
      if (line[i] == '#') {
        hashtag_exist++;
      }
      break;
    } else if (isnumerically(line[i]) || line[i] == '.')
      str.push_back(line[i]);
    else if (line[i] == 'e' || line[i] == 'E') {  // для 0.31E1 == 3.1
      if (i + 1 < line.length() && !isnumerically(line[i + 1]))
        throw std::logic_error("Invalid argument");
      str.push_back(line[i]);
    } else {
      throw std::logic_error("Invalid argument");
    }
  }
  return myconv(str, '.');
};

std::vector<std::string> QtFileReader::SplitStringByAnyDelimiter(
    const std::string &str, const std::string &delimiters) {
  std::vector<std::string> result;
  size_t start = 0;
  size_t end = str.find_first_of(delimiters);
  while (end != std::string::npos) {
    if (end != start)  // Пропускаем пустые токены
      result.push_back(str.substr(start, end - start));
    start = end + 1;
    end = str.find_first_of(delimiters, start);
  }
  if (start < str.size()) {
    result.push_back(str.substr(start));  // Добавляем последний токен
  }
  return result;
};
}  // namespace s21
