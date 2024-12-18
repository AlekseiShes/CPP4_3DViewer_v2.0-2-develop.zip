#ifndef CPP4_3DVIEWER_controller_h
#define CPP4_3DVIEWER_controller_h

#include <any>
#include <map>

#include "../../models/headers/main_model.h"

namespace s21 {
class MainController {
 public:
  MainController(Facade *other_model) : model_(other_model) {}
  std::map<bool, std::string> LoadSceneAction(const std::string &path);

  std::map<bool, std::string> OrientSceneAction(const Orient &method,
                                                const double &x,
                                                const double &y,
                                                const double &z);

  std::map<bool, std::string> SaveSettingsAction();
  std::map<bool, std::string> LoadSettingsAction();
  std::map<bool, std::string> SetPathAction(const std::string &path);
  std::map<bool, std::string> SetSettingAction(const std::string &method_name,
                                               const double &value);
  std::map<bool, std::string> SetColorAction(const std::string &method_name,
                                             const double &x, const double &y,
                                             const double &z);

  std::map<bool, std::string> GetPathAction();
  std::map<bool, double> GetSettingAction(const std::string &method_name);
  std::map<bool, float> GetColorAction(const std::string &method_name);
  std::vector<std::vector<uint32_t>> GetNormalaizeFasets();
  std::vector<double> GetNormalaizeVertexes();
  const std::vector<double> &GetCurrentLocation() const;
  std::map<bool, double> GetNormalaizeMaxCoordinate();

 private:
  Facade *model_;
};
};  // namespace s21
#endif
