#include <gtest/gtest.h>

#include <chrono>
#include <random>

#include "../3D_Viewer/controllers/headers/controller.h"
#define PRESSIZION 1E-5

// #include <fstream>
// /* inline */ std::ofstream logtxt("log.txt", std::ios::app); // Лог
// сохраняется в log.txt #define LOGTEST(message) logtxt  << __FUNCTION__ << ":
// " << message << std::endl;

using namespace s21;

class SettingsTest : public ::testing::Test {
 public:
  void SetUp() override {
    QtSceneDrawer *q_scene_drawer = new QtSceneDrawer();
    DecoratorScene *decorator_scene_drawer = new DecoratorScene(q_scene_drawer);
    Facade *model = new Facade(decorator_scene_drawer);
    MainController *controller = new MainController(model);

    this->model = model;
    this->scene_drawer = q_scene_drawer;
    this->decorator_scene_drawer = decorator_scene_drawer;
    this->controller = controller;
  }

  void TearDown() override {
    delete this->scene_drawer;
    delete this->decorator_scene_drawer;
    delete this->model;
    delete this->controller;
  }
  BaseScene *scene_drawer;
  DecoratorScene *decorator_scene_drawer;
  Facade *model;
  MainController *controller;
  std::map<bool, std::string> res;
};

TEST_F(SettingsTest, SetPathAction_GetPathAction) {
  res = controller->SetPathAction("object_files/car.obj");

  EXPECT_EQ(controller->GetPathAction().begin()->second,
            "object_files/car.obj");
  EXPECT_EQ(res.begin()->first, true);
};

TEST_F(SettingsTest, LoadSceneAction) {
  res = controller->LoadSceneAction("saasfsdf");

  EXPECT_EQ(res.begin()->first, false);

  res = controller->LoadSceneAction("");

  EXPECT_EQ(res.begin()->first, false);

  res = controller->LoadSceneAction("object_files/cube.obj");

  EXPECT_EQ(res.begin()->first, true);

  res = controller->LoadSceneAction("object_files/cottage_obj.obj");

  EXPECT_EQ(res.begin()->first, true);

  res = controller->LoadSceneAction("object_files/empty.obj");

  EXPECT_EQ(res.begin()->first, false);
  // see /models/sources/qt_normalization_parameters_model.cc:14

  res = controller->LoadSceneAction("tests/test.cc");

  EXPECT_EQ(res.begin()->first, false);
};

TEST_F(SettingsTest, FileReaderTests) {
  res = controller->LoadSceneAction("object_files/test_error3D.obj");

  EXPECT_EQ(res.begin()->first, false);

  res = controller->LoadSceneAction("object_files/test_errore.obj");  ////

  EXPECT_EQ(res.begin()->first, true);

  res = controller->LoadSceneAction("object_files/cube_tab.obj");

  EXPECT_EQ(res.begin()->first, true);

  res = controller->LoadSceneAction("object_files/test_e.obj");  ///////

  EXPECT_EQ(res.begin()->first, true);

  res = controller->LoadSceneAction("object_files/test_invalid_arg.obj");

  EXPECT_EQ(res.begin()->first, false);

  res = controller->LoadSceneAction("object_files/test_faset.obj");

  EXPECT_EQ(res.begin()->first, true);
};

class Affine_Test : public ::testing::Test {
 public:
  void SetUp() override {
    QtSceneDrawer *q_scene_drawer = new QtSceneDrawer();
    DecoratorScene *decorator_scene_drawer = new DecoratorScene(q_scene_drawer);
    Facade *model = new Facade(decorator_scene_drawer);
    MainController *controller = new MainController(model);

    this->model = model;
    this->scene_drawer = q_scene_drawer;
    this->decorator_scene_drawer = decorator_scene_drawer;
    this->controller = controller;
  }

  void TearDown() override {
    delete this->scene_drawer;
    delete this->decorator_scene_drawer;
    delete this->model;
    delete this->controller;
  }
  BaseScene *scene_drawer;
  DecoratorScene *decorator_scene_drawer;
  Facade *model;
  MainController *controller;
  std::map<bool, std::string> res;
};

TEST_F(Affine_Test, FileCubeReadAndAffine_Move) {
  res = controller->LoadSceneAction("object_files/cube.obj");

  EXPECT_EQ(res.begin()->first, true);

  std::vector<Vertex> old_vertexes =
      model->GetScene().GetFigure().GetVertexes();

  std::random_device rd;   // Источник энтропии
  std::mt19937 gen(rd());  // Генератор случайных чисел
  std::uniform_real_distribution<> dis(-1.0, 1.0);  // Диапазон от 0 до 1
  Point3D move(dis(gen), dis(gen), dis(gen));

  this->controller->OrientSceneAction(Orient::Move, move.x_, move.y_, move.z_);

  // std::vector<Vertex> new_vertexes =
  //     model->GetScene().GetFigure().GetVertexes();
  std::vector<double> new_location =
      model->GetScene().GetFigure().GetLocation();

  bool res = true;
  for (size_t i = 0; i < old_vertexes.size() && i * 3 + 2 < new_location.size();
       i++) {
    Point3D new_pos(new_location[i * 3], new_location[i * 3 + 1],
                    new_location[i * 3 + 2]);
    res = PRESSIZION > length(new_pos - old_vertexes[i].GetPosition() - move);
    if (!res) break;
  }

  EXPECT_EQ(res, true);

  this->controller->OrientSceneAction(Orient::Reset, 0, 0, 0);
  TransformMatrix Total_TransMatrix = model->GetTransformMatrix();
  // Lets check it!
  res = true;
  for (int i = 0; i < Total_TransMatrix.Matrix.getRows(); i++)
    for (int j = 0; j < Total_TransMatrix.Matrix.getRows(); j++) {
      res = PRESSIZION > fabs(Total_TransMatrix.Matrix(i, j) - (i == j));
      if (!res) break;
    }
  EXPECT_EQ(res, true);
};

// TEST_F(Affine_Test, FileMillionReadAndAffineTrans_Timings) {
//   std::chrono::duration<double, std::milli> elapsed_ms;
//   auto start = std::chrono::high_resolution_clock::now();
//   res = controller->LoadSceneAction("object_files/one_million.obj");
//   auto end = std::chrono::high_resolution_clock::now();
//   if (res.begin()->first){
//     elapsed_ms = end - start;
//     std::cout << "Execution time: LoadSceneAction(one_million.obj): " <<
//     elapsed_ms.count() << " ms" << std::endl;
//   }
//   EXPECT_EQ(res.begin()->first, true);

//   start = std::chrono::high_resolution_clock::now();
//   this->controller->OrientSceneAction(Orient::Scale, 0.1, 10, -3);
//   this->controller->OrientSceneAction(Orient::Rotation, 0, 0.5*M_PI_2,
//   -M_PI_2); this->controller->OrientSceneAction(Orient::Move, -1, 1, 0); end
//   = std::chrono::high_resolution_clock::now(); elapsed_ms = end - start;
//   std::cout << "Execution time: triple OrientSceneAction(one_million.obj): "
//   << elapsed_ms.count() << " ms" << std::endl;
// }

TEST_F(Affine_Test, FileCubeReadAndAffine_Rotation) {
  res = controller->LoadSceneAction("object_files/cube.obj");
  EXPECT_EQ(res.begin()->first, true);

  Point3D rotaX_up(M_PI_2, 0, 0);
  this->controller->OrientSceneAction(Orient::Rotation, rotaX_up.x_,
                                      rotaX_up.y_, rotaX_up.z_);
  TransformMatrix TransMatrix_rotaX_up = model->GetTransformMatrix();
  Point3D rotaZ_down(0, 0, -M_PI_2);
  this->controller->OrientSceneAction(Orient::Rotation, rotaZ_down.x_,
                                      rotaZ_down.y_, rotaZ_down.z_);
  TransformMatrix TransMatrix_rotaZ_down = model->GetTransformMatrix();
  Point3D rotaY_down(0, -M_PI_2, 0);
  this->controller->OrientSceneAction(Orient::Rotation, rotaY_down.x_,
                                      rotaY_down.y_, rotaY_down.z_);
  TransformMatrix TransMatrix_rotaY_down = model->GetTransformMatrix();
  Point3D rotaZ_up(0, 0, M_PI_2);
  this->controller->OrientSceneAction(Orient::Rotation, rotaZ_up.x_,
                                      rotaZ_up.y_, rotaZ_up.z_);
  TransformMatrix TransMatrix_rotaZ_up = model->GetTransformMatrix();
  // Total_TransMatrix should be like E-matrix.
  TransformMatrix Total_TransMatrix =
      TransMatrix_rotaX_up * TransMatrix_rotaZ_down * TransMatrix_rotaY_down *
      TransMatrix_rotaZ_up;
  // Lets check it!
  bool res = true;
  for (int i = 0; i < Total_TransMatrix.Matrix.getRows(); i++)
    for (int j = 0; j < Total_TransMatrix.Matrix.getRows(); j++) {
      res = PRESSIZION > fabs(Total_TransMatrix.Matrix(i, j) - (i == j));
      if (!res) break;
    }
  EXPECT_EQ(res, true);

  S21Matrix t(4, 4);
  t(0, 0) = t(1, 1) = t(2, 2) = t(3, 3) = 2;
  Point4D P(1, 7, 3, 5), Pcheck(2, 14, 6, 10);
  EXPECT_EQ(t * P == Pcheck, true);
};

TEST_F(Affine_Test, FileCubeReadAndAffine_Scale) {
  res = controller->LoadSceneAction("object_files/cube.obj");
  EXPECT_EQ(res.begin()->first, true);

  std::random_device rd;   // Источник энтропии
  std::mt19937 gen(rd());  // Генератор случайных чисел
  std::uniform_real_distribution<> dis(-1.0, 1.0);  // Диапазон от 0 до 1

  // лямбда-функция
  auto rand_not_zero =
      [&]() -> double {  // [&] — захват всех переменных по ссылке.
    double res = 0;
    while (fabs(res) < PRESSIZION) res = dis(gen);
    return res * 66.6;  //
  };

  double x, y, z = 1;
  Point3D cur_Scale(1, 1, 1);
  TransformMatrix Total_TransMatrix;

  for (size_t i = 0; i < 5; i++) {
    cur_Scale.x_ *= (x = rand_not_zero());
    cur_Scale.y_ *= (y = rand_not_zero());
    cur_Scale.z_ *= (z = rand_not_zero());
    this->controller->OrientSceneAction(Orient::Scale, x, y, z);
    Total_TransMatrix = Total_TransMatrix * model->GetTransformMatrix();
  }
  cur_Scale.x_ = (cur_Scale.x_ > PRECISION) ? 1 / cur_Scale.x_ : 1;
  cur_Scale.y_ = (cur_Scale.y_ > PRECISION) ? 1 / cur_Scale.y_ : 1;
  cur_Scale.z_ = (cur_Scale.z_ > PRECISION) ? 1 / cur_Scale.z_ : 1;
  this->controller->OrientSceneAction(Orient::Scale, cur_Scale.x_, cur_Scale.y_,
                                      cur_Scale.z_);
  Total_TransMatrix = Total_TransMatrix * model->GetTransformMatrix();
  // Total_TransMatrix should be like E-matrix.

  // Lets check it!
  bool res = true;
  for (int i = 0; i < Total_TransMatrix.Matrix.getRows(); i++)
    for (int j = 0; j < Total_TransMatrix.Matrix.getRows(); j++) {
      res = PRESSIZION > fabs(Total_TransMatrix.Matrix(i, j) - (i == j));
      if (!res) break;
    }
  EXPECT_EQ(res, true);
};

TEST_F(SettingsTest, SaveSettingsAction_LoadSettingsAction) {
  std::string q_path_old = "object_files/cube_tab.obj";
  controller->SetPathAction(q_path_old);
  res = controller->SaveSettingsAction();
  EXPECT_EQ(res.begin()->first, true);

  controller->SetPathAction("");
  res = controller->LoadSettingsAction();
  EXPECT_EQ(res.begin()->first, true);

  std::string q_path_new = controller->GetPathAction().begin()->second;

  EXPECT_EQ(res.begin()->first, true);
  EXPECT_EQ(q_path_old, q_path_new);
};

TEST_F(SettingsTest, LoadSettingsAction_Error) {
  res = controller->SaveSettingsAction();  // пораждает файл если его нет
  EXPECT_EQ(res.begin()->first, true);
  if (res.begin()->first) {
    chmod("settings.txt", 0000);  // Полный запрет доступа
    res = controller->LoadSettingsAction();
    chmod("settings.txt", 0644);  // Восстанавливаем права доступа
    EXPECT_EQ(res.begin()->first, true);
    EXPECT_EQ(res.begin()->second.find("not open") != std::string::npos, true);
  }
};

TEST_F(SettingsTest, SaveSettingsAction_Error) {
  // Создаем файл и запрещаем запись
  std::ofstream out("settings.txt");
  out.close();

  chmod("settings.txt", 0444);  // Только для чтения
  res = controller->SaveSettingsAction();
  chmod("settings.txt", 0644);  // Восстанавливаем права доступа

  EXPECT_EQ(res.begin()->first, false);
};

TEST_F(SettingsTest, SetSettingAction_GetSettingAction) {
  res = controller->SetSettingAction("x_shift", 12);

  EXPECT_EQ(res.begin()->first, true);

  EXPECT_EQ(this->controller->GetSettingAction("x_shift").begin()->second, 12);

  res = controller->SetSettingAction("x_shift", std::nan(""));

  EXPECT_EQ(res.begin()->first, false);

  EXPECT_EQ(this->controller->GetSettingAction("test").begin()->first, false);
};

TEST_F(SettingsTest, SetColorAction_GetColorAction) {
  res = controller->SetColorAction("vertex_color", 1.0, 0.0, 0.0);

  EXPECT_EQ(res.begin()->first, true);

  EXPECT_EQ(
      this->controller->GetColorAction("red_vertex_color").begin()->second,
      1.0);

  res = controller->SetColorAction("vertex_color", std::nan(""), 0.0, 0.0);

  EXPECT_EQ(res.begin()->first, false);

  res = controller->SetColorAction("vertex_color", 1.0, std::nan(""), 0.0);

  EXPECT_EQ(res.begin()->first, false);

  res = controller->SetColorAction("vertex_color", 1.0, 0.0, std::nan(""));

  EXPECT_EQ(res.begin()->first, false);

  res = controller->SetColorAction("test", 1.0, 0.0, 0.0);

  EXPECT_EQ(res.begin()->first, false);

  res = controller->SetColorAction("", 1.0, 0.0, 0.0);

  EXPECT_EQ(res.begin()->first, false);

  EXPECT_EQ(this->controller->GetColorAction("test").begin()->first, false);
};

TEST_F(SettingsTest, GetNormalaizeFasets) {
  std::vector<std::vector<uint32_t>> faset =
      this->controller->GetNormalaizeFasets();

  EXPECT_EQ(faset.size(), 0);

  res = controller->LoadSceneAction("object_files/cube.obj");

  faset = this->controller->GetNormalaizeFasets();

  ASSERT_TRUE(faset.size() != 0);

  res = controller->LoadSceneAction("object_files/empty.obj");

  faset = this->controller->GetNormalaizeFasets();

  ASSERT_FALSE(faset.size() != 0);
};

TEST_F(SettingsTest, GetNormalaizeVertexes) {
  std::vector<double> vertex = this->controller->GetNormalaizeVertexes();

  EXPECT_EQ(vertex.size(), 0);

  res = controller->LoadSceneAction("object_files/cube.obj");

  vertex = this->controller->GetNormalaizeVertexes();

  ASSERT_TRUE(vertex.size() != 0);

  res = controller->LoadSceneAction("object_files/empty.obj");

  vertex = this->controller->GetNormalaizeVertexes();

  ASSERT_FALSE(vertex.size() != 0);
};

TEST_F(SettingsTest, GetNormalaizeMaxCoordinate) {
  double max_coordinate =
      this->controller->GetNormalaizeMaxCoordinate().begin()->second;

  EXPECT_EQ(max_coordinate, 0.0);

  res = controller->LoadSceneAction("object_files/cube.obj");

  EXPECT_EQ(res.begin()->first, true);

  double max_coordinate_2 =
      this->controller->GetNormalaizeMaxCoordinate().begin()->second;

  ASSERT_TRUE(max_coordinate != max_coordinate_2);
};

TEST_F(SettingsTest, Class_Point4D) {
  S21Matrix m(4, 4);
  int v = 0;
  for (int r = 0; r < m.getRows(); ++r)
    for (int c = 0; c < m.getRows(); ++c) m(r, c) = v++;
  Point4D P4_old(2, 1, 0, 1);
  Point4D P4_new = m * P4_old;
  Point4D P4_check(4, 20, 36, 52);
  EXPECT_EQ(P4_check == P4_new, true);
  EXPECT_EQ(P4_check.to_string(1), "(4.0; 20.0; 36.0; 52.0)");
  EXPECT_EQ(m.to_string(), m.to_string(6));

  S21Matrix m2(3, 4);
  m2.setCols(3);
  m2(0, 0) = m2(1, 1) = 1;
  m2.setValue(2, 2, 1);
  S21Matrix m2_moved = std::move(m2);
  S21Matrix m2_new = m2_moved * m2_moved / m2_moved;
  EXPECT_EQ(m2_new.to_string(3), m2_moved.to_string(3));

  m2_new = m2_new + 3.0 * m2_moved + m2_moved * 1.0 - m2_moved / 0.25;
  m2_moved *= 10;
  m2_moved /= 10;
  m2_new -= m2_moved;
  m2_new += m2_moved;
  m2_new /= m2_moved;
  m2_new *= 1 / (1 / m2_moved);
  EXPECT_EQ(m2_new == m2_moved, true);

  Point3D a(0, 1, 2);
  Point3D b;
  b = a;
  EXPECT_EQ(a.to_string(), b.to_string(6));

  // Проверяем, что происходит выброс исключения
  EXPECT_ANY_THROW({ auto result = m2 * P4_old; });
};

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};

//   if
//   (this->controller_->GetSettingAction("type_of_vertexes").begin()->second) {
//     glEnable(GL_POINT_SMOOTH);
//     // glutSolidSphere();
//   } else {
//     glDisable(GL_POINT_SMOOTH);
//   }

//   /  src/tests.cc
//   Point3D c(1, 2, 2);
//   Point3D f;
//   f.y_ = 2;
//   Point3D a = c-f;
//   f = a;
//   c = std::move(a);
//   std::cout << "a.to_string()=: " << a.to_string() << std::endl;
//   std::cout << "length: " << std::to_string(length(a)) << std::endl;
//   Point4D d(1, 2, 3);
//   Point4D e(1, 0,0,0);
//   Point4D b = d - e;
//   d = b;
//   e = b;
//   // e = std::move(b);
//   std::cout << "b.to_string()=: " << b.to_string() << std::endl;
//   std::cout << "length: " << std::to_string(length(b)) << std::endl;
