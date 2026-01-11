#pragma once

#include <QOpenGLWidget>

class Qtgl : public QOpenGLWidget {
protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int w, int h) override;
  virtual void paintGL() override;

public:
  Qtgl(QWidget *pwdj);
};
