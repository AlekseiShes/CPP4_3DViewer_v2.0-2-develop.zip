#include "../headers/mainwindow.h"

#include "../../models/headers/transformmatrix_model.h"
#include "ui_mainwindow.h"

namespace s21 {
CPP4_3D_Viewer_v2::CPP4_3D_Viewer_v2(MainController *controller,
                                     QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::CPP4_3D_Viewer_v2),
      controller_(controller) {
  ui->setupUi(this);

  screen_ = new QImage;
  ui->opengl_widget->SetController(this->controller_);

  spin["x_rotate"] = ui->x_rotate;  // Заполняем map
  spin["y_rotate"] = ui->y_rotate;
  spin["z_rotate"] = ui->z_rotate;
  spin["x_shift"] = ui->x_shift;
  spin["y_shift"] = ui->y_shift;
  spin["z_shift"] = ui->z_shift;
  spin["x_scale"] = ui->x_scale;
  spin["y_scale"] = ui->y_scale;
  spin["z_scale"] = ui->z_scale;
  ui->opengl_widget->SendPointer(spin);  // передаем в gl

  if (!this->controller_->GetPathAction().begin()->second.empty()) {
    DrawScene();
  }
  SetSettingOnWindow();
  for (auto &[key, spins] : spin) {
    connect(spins, SIGNAL(valueChanged(double)), this,
            SLOT(SetSetting(double)));
  }
  connect(ui->thickness_of_edges, SIGNAL(valueChanged(double)), this,
          SLOT(SetSetting(double)));
  connect(ui->size_of_vertexes, SIGNAL(valueChanged(double)), this,
          SLOT(SetSetting(double)));
  connect(ui->type_of_rotate, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetType(int)));
  connect(ui->type_of_projection, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetType(int)));
  connect(ui->type_of_edges, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetType(int)));
  connect(ui->type_of_vertexes, SIGNAL(currentIndexChanged(int)), this,
          SLOT(SetType(int)));
};

CPP4_3D_Viewer_v2::~CPP4_3D_Viewer_v2() {
  this->controller_->SaveSettingsAction();
  if (this->gif_) {
    delete this->gif_;
    this->gif_ = nullptr;
  }

  delete this->screen_;

  if (this->gif_timer_) {
    delete this->gif_timer_;
    this->gif_timer_ = nullptr;
  }
  delete this->w;
  delete ui;
};

void CPP4_3D_Viewer_v2::on_open_file_clicked() {
  QString path = QFileDialog::getOpenFileName(
      this, "Выбрать файл объекта", "../object_files", tr("Objects (*.obj)"));
  if (!path.isEmpty()) {
    on_set_default_clicked();  // сбрасываем настройки
    this->controller_->SetPathAction(path.toStdString());
    DrawScene();
    InfoAboutObject();
  }
};

void CPP4_3D_Viewer_v2::on_vertex_color_clicked() {
  QColor vertex_color =
      QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (vertex_color.isValid()) {
    this->controller_->SetColorAction(
        ui->vertex_color->objectName().toStdString(), vertex_color.redF(),
        vertex_color.greenF(), vertex_color.blueF());
  }
};

void CPP4_3D_Viewer_v2::on_line_color_clicked() {
  QColor line_color = QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (line_color.isValid()) {
    this->controller_->SetColorAction(
        ui->line_color->objectName().toStdString(), line_color.redF(),
        line_color.greenF(), line_color.blueF());
  }
};

void CPP4_3D_Viewer_v2::on_background_color_clicked() {
  QColor background_color =
      QColorDialog::getColor(Qt::white, this, "Choose a color");
  if (background_color.isValid()) {
    this->controller_->SetColorAction(
        ui->background_color->objectName().toStdString(),
        background_color.redF(), background_color.greenF(),
        background_color.blueF());
  }
};

void CPP4_3D_Viewer_v2::SetSetting(double value) {
  if (!((QDoubleSpinBox *)sender())->isEnabled()) return;
  std::string name = ((QDoubleSpinBox *)sender())->objectName().toStdString();
  controller_->SetSettingAction(name, value);
  if (name.find("shift") != std::string::npos) {
    controller_->OrientSceneAction(
        Orient::Move, controller_->GetSettingAction("x_shift").begin()->second,
        controller_->GetSettingAction("y_shift").begin()->second,
        controller_->GetSettingAction("z_shift").begin()->second);
  } else if (name.find("scale") != std::string::npos) {
    controller_->OrientSceneAction(
        Orient::Scale, controller_->GetSettingAction("x_scale").begin()->second,
        controller_->GetSettingAction("y_scale").begin()->second,
        controller_->GetSettingAction("z_scale").begin()->second);
  } else if (name.find("rotate") != std::string::npos) {
    controller_->OrientSceneAction(
        Orient::Rotation,
        controller_->GetSettingAction("x_rotate").begin()->second,
        controller_->GetSettingAction("y_rotate").begin()->second,
        controller_->GetSettingAction("z_rotate").begin()->second);
  }
};

void CPP4_3D_Viewer_v2::SetType(int value) {
  QComboBox *object_name = (QComboBox *)sender();

  this->controller_->SetSettingAction(object_name->objectName().toStdString(),
                                      value);
  if (object_name->objectName() == "type_of_rotate" && value == 0) {
    for (auto &[key, spins] : spin) {
      spins->setEnabled(true);
      spins->setValue(this->controller_->GetSettingAction(key).begin()->second);
    }
  } else if (object_name->objectName() == "type_of_rotate" && value == 1) {
    for (auto &[key, spins] : spin) {
      spins->setEnabled(false);
    }
  }
  if (ui->type_of_vertexes->currentIndex() == 2) {
    ui->size_of_vertexes->setMaximum(1);
  } else {
    ui->size_of_vertexes->setMinimum(1);
    ui->size_of_vertexes->setMaximum(100);
  }
};

void CPP4_3D_Viewer_v2::DrawScene() {
  this->controller_->LoadSceneAction(
      this->controller_->GetPathAction().begin()->second);
  ui->opengl_widget->StartDrawer();
};

void CPP4_3D_Viewer_v2::SetSettingOnWindow() {
  ui->thickness_of_edges->setValue(
      this->controller_->GetSettingAction("thickness_of_edges")
          .begin()
          ->second);
  ui->size_of_vertexes->setValue(
      this->controller_->GetSettingAction("size_of_vertexes").begin()->second);
  ui->type_of_rotate->setCurrentIndex(
      this->controller_->GetSettingAction("type_of_rotate").begin()->second);

  for (auto &[key, spins] : spin) {
    spins->setValue(this->controller_->GetSettingAction(key).begin()->second);
    spins->setEnabled(ui->type_of_rotate->currentIndex() == 0);
  }

  controller_->OrientSceneAction(
      Orient::Move, controller_->GetSettingAction("x_shift").begin()->second,
      controller_->GetSettingAction("y_shift").begin()->second,
      controller_->GetSettingAction("z_shift").begin()->second);
  controller_->OrientSceneAction(
      Orient::Scale, controller_->GetSettingAction("x_scale").begin()->second,
      controller_->GetSettingAction("y_scale").begin()->second,
      controller_->GetSettingAction("z_scale").begin()->second);
  controller_->OrientSceneAction(
      Orient::Rotation,
      controller_->GetSettingAction("x_rotate").begin()->second,
      controller_->GetSettingAction("y_rotate").begin()->second,
      controller_->GetSettingAction("z_rotate").begin()->second);

  ui->type_of_projection->setCurrentIndex(
      this->controller_->GetSettingAction("type_of_projection")
          .begin()
          ->second);
  ui->type_of_edges->setCurrentIndex(
      this->controller_->GetSettingAction("type_of_edges").begin()->second);
  ui->type_of_vertexes->setCurrentIndex(
      this->controller_->GetSettingAction("type_of_vertexes").begin()->second);
  InfoAboutObject();
};

void CPP4_3D_Viewer_v2::on_set_default_clicked() {
  for (auto &[key, spins] : spin) {
    spins->setValue(key.find("_scale") != std::string::npos);
    spins->setEnabled(false);
  }
  controller_->OrientSceneAction(Orient::Reset, 0, 0, 0);

  // ui->x_shift->setValue(0);
  // ui->y_shift->setValue(0);
  // ui->z_shift->setValue(0);
  // ui->x_scale->setValue(1);
  // ui->y_scale->setValue(1);
  // ui->z_scale->setValue(1);
  // ui->x_rotate->setValue(0);
  // ui->y_rotate->setValue(0);
  // ui->z_rotate->setValue(0);

  ui->type_of_rotate->setCurrentIndex(1);  // mouse

  ui->thickness_of_edges->setValue(0);
  ui->size_of_vertexes->setValue(0);
  ui->type_of_projection->setCurrentIndex(1);
  ui->type_of_edges->setCurrentIndex(0);
  ui->type_of_vertexes->setCurrentIndex(2);

  ui->info_about_object->setText("Загрузите объект");

  this->controller_->SetPathAction("");
  this->controller_->SetColorAction("vertex_color", 1.0, 0.0, 0.0);
  this->controller_->SetColorAction("line_color", 0.596, 1.0, 0.133);
  this->controller_->SetColorAction("background_color", 0, 0, 0);
};

void CPP4_3D_Viewer_v2::InfoAboutObject() {
  QString name_of_object = QString::fromStdString(
      this->controller_->GetPathAction().begin()->second);
  if (name_of_object.isEmpty()) {
    this->ui->info_about_object->setText("Загрузите объект");
  } else {
    name_of_object.remove(0, (name_of_object.lastIndexOf('/') + 1));
    this->ui->info_about_object->setText(
        "Название:  " + name_of_object + "\nКоличество вершин:  " +
        QString::number(this->controller_->GetCurrentLocation().size() / 3) +
        "\nКоличество полигонов:  " +
        QString::number(this->controller_->GetNormalaizeFasets().size()));
  }
};

void CPP4_3D_Viewer_v2::on_screenshot_clicked() {
  *screen_ = ui->opengl_widget->grabFramebuffer();
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", QDir::currentPath() + "/../save.bmp",
      "BMP (*.bmp);; JPEG (*.jpeg)");
  screen_->save(file);
};

void CPP4_3D_Viewer_v2::on_gif_clicked() {
  ui->gif->setEnabled(false);
  ui->gif->setText("Recording");
  ui->gif->setStyleSheet("QPushButton {font-weight: bold; color: red;}");
  gif_ = new QGifImage(QSize(640, 480));
  gif_->setDefaultDelay(100);
  time_ = 0;
  gif_timer_ = new QTimer(this);
  connect(gif_timer_, SIGNAL(timeout()), this, SLOT(MakeGif()));
  gif_timer_->start(100);
};

void CPP4_3D_Viewer_v2::MakeGif() {
  ++time_;
  *screen_ = ui->opengl_widget->grabFramebuffer();
  *screen_ = screen_->scaled(640, 480, Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation);
  gif_->addFrame(*screen_, 100);
  if (time_ == 50) {
    gif_timer_->stop();
    QString path = QFileDialog::getSaveFileName(
        this, tr("Save screenshot"), QDir::currentPath() + "/../save.gif",
        tr("GIF screenshot (*.gif)"));
    gif_->save(path);
    delete gif_;
    delete gif_timer_;
    gif_ = nullptr;
    gif_timer_ = nullptr;
    ui->gif->setEnabled(true);
    ui->gif->setStyleSheet("QPushButton {color: black;}");
    ui->gif->setText("Start");
  }
};

}  // namespace s21
