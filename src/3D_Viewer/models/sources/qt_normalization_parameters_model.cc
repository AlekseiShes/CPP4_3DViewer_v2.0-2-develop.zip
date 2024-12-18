#include "../headers/main_model.h"

namespace s21 {

// void
Point3D QtNormalizationParameters::NormalaizeParametersScene(
    const Scene &scene) {
  Point3D res = NormalaizeVertexes(scene);
  NormalaizeFacets(scene);
  return res;
};

// void
Point3D QtNormalizationParameters::NormalaizeVertexes(const Scene &scene) {
  if (!scene.GetFigure().GetVertexes().size())
    throw std::logic_error("scene is empty");
  std::vector<Vertex> tmp_vertexes = scene.GetFigure().GetVertexes();
  auto init_it = tmp_vertexes.begin();
  Point3D max = init_it->GetPosition(), min = max;

  for (auto it = tmp_vertexes.begin(); it != tmp_vertexes.end(); ++it)
    for (size_t i = 0; i < max.dimension(); ++i) {
      if (it->GetPosition()[i] > max[i]) max[i] = it->GetPosition()[i];
      if (it->GetPosition()[i] < min[i]) min[i] = it->GetPosition()[i];
    }

  Point3D shift = (max + min) * (-0.5);
  this->max_coordinate_ = std::max(
      std::fabs(shift.x_ + max.x_),
      std::max(std::fabs(shift.y_ + max.y_), std::fabs(shift.z_ + max.z_)));

  for (auto it = tmp_vertexes.begin(); it != tmp_vertexes.end(); ++it)
    for (size_t i = 0; i < max.dimension(); ++i)
      vertex_normal_.push_back(it->GetPosition()[i] + shift[i]);

  return shift;
};

void QtNormalizationParameters::NormalaizeFacets(const Scene &scene) {
  if (!scene.GetFigure().GetEdges().size())
    return;  //  throw std::logic_error("scene is empty");

  std::vector<int> one_facet;
  std::vector<uint32_t> one_normal_facet;

  std::vector<Edge> tmp_facets = scene.GetFigure().GetEdges();
  int correct_value = scene.GetFigure().GetVertexes().size() / 3 + 1;

  for (size_t i{}; i < tmp_facets.size(); ++i) {
    for (size_t j{}; j < tmp_facets[i].GetEdge().size(); ++j) {
      one_facet.push_back(tmp_facets[i].GetEdge()[j] - 1);
      if (j != tmp_facets[i].GetEdge().size() - 1) {
        one_facet.push_back(tmp_facets[i].GetEdge()[j + 1] - 1);
      } else {
        one_facet.push_back(tmp_facets[i].GetEdge()[0] - 1);
      }
    }
    if (one_facet[0] < 0) {
      for (size_t j{}; j < one_facet.size(); ++j) {
        one_normal_facet.push_back(one_facet[j] + correct_value);
      }
    } else {
      for (size_t j{}; j < one_facet.size(); ++j) {
        one_normal_facet.push_back(one_facet[j]);
      }
    }
    facet_normal_.push_back(one_normal_facet);
    one_normal_facet.clear();
    one_facet.clear();
  }
};

std::any QtNormalizationParameters::GetVertexes() const {
  return this->vertex_normal_;
};

std::any QtNormalizationParameters::GetFacets() const {
  return this->facet_normal_;
};

std::any QtNormalizationParameters::GetMaxCoordinate() const {
  return this->max_coordinate_;
};

void QtNormalizationParameters::clearParams() {
  this->facet_normal_.clear();
  this->vertex_normal_.clear();
  this->max_coordinate_ = 0;
};
}  // namespace s21
