#ifndef CPP4_3DVIEWER_VISUALIZATION3D_H
#define CPP4_3DVIEWER_VISUALIZATION3D_H

#include <QDoubleSpinBox>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWidget>

#include "../../controllers/headers/controller.h"
#include "../headers/mainwindow.h"

namespace s21 {
class Visualization3D : public QOpenGLWidget {
  Q_OBJECT
 public:
  explicit Visualization3D(QWidget *parent = nullptr);
  ~Visualization3D();
  void SetController(MainController *controller);
  void SendPointer(std::map<std::string, QDoubleSpinBox *> spin);
  void StartDrawer();

 private:
  void mousePressEvent(QMouseEvent *mo) override;
  void mouseMoveEvent(QMouseEvent *mo) override;
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void Draw();

  MainController *controller_{};
  QTimer gl_timer_;
  QPoint mPos_;
  double max_ = 0;
  std::vector<std::vector<uint32_t>> f_vector_;
  std::map<std::string, QDoubleSpinBox *> spin_;
};
}  // namespace s21
#endif
