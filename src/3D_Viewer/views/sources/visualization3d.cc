#include "../headers/visualization3d.h"
#define SUM_SPIN(spinBox, delta) spinBox->setValue(spinBox->value() + delta)
#define MUL_SPIN(spinBox, delta) spinBox->setValue(spinBox->value() * delta)

namespace s21 {

Visualization3D::Visualization3D(QWidget *parent) : QOpenGLWidget(parent) {
  connect(&gl_timer_, SIGNAL(timeout()), this, SLOT(update()));
  gl_timer_.start(50);
};

Visualization3D::~Visualization3D() { this->f_vector_.clear(); };

void Visualization3D::initializeGL() { glEnable(GL_DEPTH_TEST); };

void Visualization3D::paintGL() {
  glClearColor(
      this->controller_->GetColorAction("red_background_color").begin()->second,
      this->controller_->GetColorAction("green_background_color")
          .begin()
          ->second,
      this->controller_->GetColorAction("blue_background_color")
          .begin()
          ->second,
      1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  unsigned v = 1;
  if (this->controller_->GetSettingAction("type_of_projection")
          .begin()
          ->second == 0) {
    glOrtho(-max_ * v, max_ * v, -max_ * v, max_ * v, max_ * v, v * 5 * max_);
  } else {
    glFrustum(-max_ * v, max_ * v, -max_ * v, max_ * v, max_ * v, v * 5 * max_);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // glTranslated(0, 0, -2.8 * this->max_);
  glTranslated(0, 0, -2.8 * this->max_);

  for (auto &[key, spins] : spin_) {
    if (spins) this->controller_->SetSettingAction(key, spins->value());
  }

  if (!this->controller_->GetPathAction().begin()->second.empty()) {
    Draw();
  }
};

void Visualization3D::resizeGL(int w, int h) { glViewport(0, 0, w, h); };

void Visualization3D::mousePressEvent(QMouseEvent *mo) {
  if (this->controller_->GetSettingAction("type_of_rotate").begin()->second ==
      1) {
    mPos_ = mo->pos();
  }
};

void Visualization3D::mouseMoveEvent(QMouseEvent *mo) {
  if (this->controller_->GetSettingAction("type_of_rotate").begin()->second !=
      1)
    return;
  int chaged_rotate = 0;
  if (spin_["x_rotate"] && spin_["y_rotate"] &&
      mo->buttons() & Qt::LeftButton && !(mo->buttons() & Qt::MiddleButton) &&
      !(mo->buttons() & Qt::RightButton)) {
    SUM_SPIN(spin_["x_rotate"], 1 / M_PI * (mo->pos().y() - mPos_.y()));
    SUM_SPIN(spin_["y_rotate"], 1 / M_PI * (mo->pos().x() - mPos_.x()));
    ++chaged_rotate;
  }
  if (spin_["z_rotate"] && mo->buttons() & Qt::LeftButton &&
      mo->buttons() & Qt::MiddleButton && !(mo->buttons() & Qt::RightButton)) {
    SUM_SPIN(spin_["z_rotate"], 1 / M_PI * (mo->pos().y() - mPos_.y()));
    ++chaged_rotate;
  }
  if (chaged_rotate)
    controller_->OrientSceneAction(Orient::Rotation, spin_["x_rotate"]->value(),
                                   spin_["y_rotate"]->value(),
                                   spin_["z_rotate"]->value());

  if (spin_["x_scale"] && spin_["y_scale"] && spin_["z_scale"] &&
      mo->buttons() & Qt::MiddleButton && !(mo->buttons() & Qt::LeftButton) &&
      !(mo->buttons() & Qt::RightButton)) {
    double coeff = pow(1.1, 0.1 * (mo->pos().y() - mPos_.y()));
    MUL_SPIN(spin_["x_scale"], coeff);
    MUL_SPIN(spin_["y_scale"], coeff);
    MUL_SPIN(spin_["z_scale"], coeff);
    controller_->OrientSceneAction(Orient::Scale, spin_["x_scale"]->value(),
                                   spin_["y_scale"]->value(),
                                   spin_["z_scale"]->value());
  }
  int changed_shift = 0;
  if (spin_["x_shift"] && spin_["y_shift"] && mo->buttons() & Qt::RightButton &&
      !(mo->buttons() & Qt::MiddleButton) &&
      !(mo->buttons() & Qt::LeftButton)) {
    SUM_SPIN(spin_["x_shift"], 0.01 * (mo->pos().x() - mPos_.x()));
    SUM_SPIN(spin_["y_shift"], -0.01 * (mo->pos().y() - mPos_.y()));
    ++changed_shift;
  }
  if (spin_["z_shift"] && mo->buttons() & Qt::RightButton &&
      mo->buttons() & Qt::MiddleButton && !(mo->buttons() & Qt::LeftButton)) {
    SUM_SPIN(spin_["z_shift"], 0.01 * (mo->pos().y() - mPos_.y()));
    ++changed_shift;
  }
  if (changed_shift)
    controller_->OrientSceneAction(Orient::Move, spin_["x_shift"]->value(),
                                   spin_["y_shift"]->value(),
                                   spin_["z_shift"]->value());

  mPos_ = mo->pos();
  update();
};

void Visualization3D::Draw() {
  glVertexPointer(3, GL_DOUBLE, 0, controller_->GetCurrentLocation().data());

  glEnableClientState(GL_VERTEX_ARRAY);

  if (this->controller_->GetSettingAction("type_of_vertexes").begin()->second) {
    glEnable(GL_POINT_SMOOTH);
  }
  glColor3d(
      this->controller_->GetColorAction("red_vertex_color").begin()->second,
      this->controller_->GetColorAction("green_vertex_color").begin()->second,
      this->controller_->GetColorAction("blue_vertex_color").begin()->second);

  glDrawArrays(GL_POINTS, 0, controller_->GetCurrentLocation().size() / 3);

  glPointSize(
      this->controller_->GetSettingAction("size_of_vertexes").begin()->second);
  glDisable(GL_POINT_SMOOTH);

  if (this->controller_->GetSettingAction("type_of_edges").begin()->second) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0X00FF);
  }
  glColor3d(
      this->controller_->GetColorAction("red_line_color").begin()->second,
      this->controller_->GetColorAction("green_line_color").begin()->second,
      this->controller_->GetColorAction("blue_line_color").begin()->second);
  for (size_t i = 0; i < this->f_vector_.size(); ++i) {
    glDrawElements(GL_LINE_LOOP, this->f_vector_[i].size(), GL_UNSIGNED_INT,
                   this->f_vector_[i].data());
  }
  glLineWidth(this->controller_->GetSettingAction("thickness_of_edges")
                  .begin()
                  ->second);
  glDisable(GL_LINE_STIPPLE);

  glDisableClientState(GL_VERTEX_ARRAY);
};

void Visualization3D::SetController(MainController *controller) {
  this->controller_ = controller;
};

void Visualization3D::SendPointer(
    std::map<std::string, QDoubleSpinBox *> spin) {
  this->spin_ = spin;
};

void Visualization3D::StartDrawer() {
  this->f_vector_.clear();
  this->f_vector_ = this->controller_->GetNormalaizeFasets();
  this->max_ = this->controller_->GetNormalaizeMaxCoordinate().begin()->second;
};

}  // namespace s21
