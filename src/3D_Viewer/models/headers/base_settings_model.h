#ifndef CPP4_3DVIEWER_BASE_SETTINGS_MODEL_H
#define CPP4_3DVIEWER_BASE_SETTINGS_MODEL_H

#include <string>

namespace s21 {
class BaseSettings {
 public:
  virtual void SetPath(const std::string &path) = 0;
  virtual void SetSetting(const std::string &method_name,
                          const double &value) = 0;
  virtual void SetColor(const std::string &method_name,
                        const Point3D &color) = 0;
  virtual std::string GetPath() const = 0;
  virtual double GetSetting(const std::string &method_name) const = 0;
  virtual float GetColor(const std::string &method_name) const = 0;
  virtual void SaveSettings() const = 0;
  virtual int LoadSettings() = 0;
};
};  // namespace s21
#endif
