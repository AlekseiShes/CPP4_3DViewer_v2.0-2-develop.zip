#ifndef CPP4_3DVIEWER_MAINWINDOW_H
#define CPP4_3DVIEWER_MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <string>

#include "../../controllers/headers/controller.h"
#include "../../models/gif_model/src/gifimage/qgifimage.h"
#include "visualization3d.h"

using namespace s21;

QT_BEGIN_NAMESPACE
namespace Ui {
class CPP4_3D_Viewer_v2;
}
QT_END_NAMESPACE

namespace s21 {
class CPP4_3D_Viewer_v2 : public QMainWindow {
  Q_OBJECT

 public:
  CPP4_3D_Viewer_v2(MainController *controller, QWidget *parent = nullptr);
  ~CPP4_3D_Viewer_v2();
  Ui::CPP4_3D_Viewer_v2 *GetUI() { return ui; }

 private slots:
  void on_open_file_clicked();
  void on_vertex_color_clicked();
  void on_line_color_clicked();
  void on_background_color_clicked();
  void on_set_default_clicked();
  void on_screenshot_clicked();
  void on_gif_clicked();
  void SetSettingOnWindow();
  void SetSetting(double value);
  void SetType(int value);
  void MakeGif();
  void InfoAboutObject();
  void DrawScene();

 private:
  Ui::CPP4_3D_Viewer_v2 *ui;
  QWidget *w;
  MainController *controller_;
  QTimer *gif_timer_ = nullptr;
  int time_{};
  QGifImage *gif_ = nullptr;
  QImage *screen_;
  std::map<std::string, QDoubleSpinBox *> spin;
};
}  // namespace s21
#endif
