#include "../headers/main_model.h"

namespace s21 {

void QtSettings::SetPath(const std::string &path) { this->path_ = path; };

std::string QtSettings::GetPath() const { return this->path_; };

QtSettings::QtSettings() { LoadSettings(); };

QtSettings::~QtSettings() { SaveSettings(); };

void QtSettings::SetSetting(const std::string &method_name,
                            const double &value) {
  this->checkMethodName(method_name);
  this->checkNumber(value);
  settings_[method_name] = value;
};

double QtSettings::GetSetting(const std::string &method_name) const {
  if (settings_.find(method_name) == settings_.end()) {
    throw std::runtime_error("Invalid method name");
  }
  return settings_.at(method_name);
};

void QtSettings::SetColor(const std::string &method_name,
                          const Point3D &color) {
  if (method_name != "vertex_color" && method_name != "line_color" &&
      method_name != "background_color")
    throw std::runtime_error("Invalid method name");

  for (size_t i = 0; i < color.dimension(); ++i) checkNumber(color[i]);

  this->colors_["red_" + method_name] = color.x_;
  this->colors_["green_" + method_name] = color.y_;
  this->colors_["blue_" + method_name] = color.z_;
};

float QtSettings::GetColor(const std::string &method_name) const {
  if (colors_.find(method_name) == colors_.end()) {
    throw std::runtime_error("Invalid method name");
  }
  return this->colors_.find(method_name)->second;
};

void QtSettings::SaveSettings() const {
  std::ofstream out;
  out.open("settings.txt");
  if (out.is_open()) {
    out << "path_=" << this->path_ << std::endl;
    for (auto it = settings_.begin(); it != settings_.end(); ++it) {
      out << it->first << "=" << it->second << std::endl;
    }
    for (auto it = colors_.begin(); it != colors_.end(); ++it) {
      out << it->first << "=" << it->second << std::endl;
    }
  }
  if (out.fail()) throw std::ios_base::failure("Failed to write to the file.");
  out.close();
};

int QtSettings::LoadSettings() {
  std::ifstream file;
  std::string str_tmp;
  file.open("settings.txt");
  if (file.is_open()) {
    std::string key;
    std::string value;
    double result = 0.0;
    while (std::getline(file, str_tmp)) {
      value = str_tmp.substr(str_tmp.find("=") + 1);
      key = str_tmp.erase(str_tmp.find("="));
      if (key == "path_") {
        this->path_ = value;
      } else if (key.find("color") != std::string::npos) {
        result = myconv(value.c_str(), '.');
        this->colors_[key] = result;
      } else {
        result = myconv(value.c_str(), '.');
        this->settings_[key] = result;
      }
    }
  } else {
    return 1;
  }
  file.close();
  return 0;
};

void QtSettings::checkMethodName(const std::string &method_name) {
  if (settings_.find(method_name) == settings_.end()) {
    throw std::runtime_error("Invalid method name");
  }
};

void QtSettings::checkNumber(const double &value) {
  if (value == 0.0) return;
  if (!std::isnormal(value) || fabs(value) > 10000.0000000)
    throw std::runtime_error("Invalid value");
};

}  // namespace s21
