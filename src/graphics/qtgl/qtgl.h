#pragma once

#include <QOpenGLWidget>
#include <qevent.h>

class Qtgl : public QOpenGLWidget {
private:
  GLuint m_nPyramid;
  GLfloat m_xRotate;
  GLfloat m_yRotate;
  GLfloat m_scale = 1.0f;
  QPoint m_ptPosition;
  bool isLeftBut = false;

protected:
  virtual void initializeGL();
  virtual void resizeGL(int nWidth, int nHeight);
  virtual void paintGL();
  virtual void mousePressEvent(QMouseEvent *pe);
  virtual void mouseMoveEvent(QMouseEvent *pe);
  virtual void wheelEvent(QWheelEvent *pe);
  GLuint createPyramid(GLfloat fSize = 1.0f);

public:
  Qtgl(QWidget *pwgt = 0);
};
