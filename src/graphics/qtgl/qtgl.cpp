#include "qtgl.h"
#include <QOpenGLFunctions>

Qtgl::Qtgl(QWidget *pwdj) : QOpenGLWidget(pwdj) {}

void Qtgl::initializeGL() {
  QOpenGLFunctions *pFunc = QOpenGLContext::currentContext()->functions();
  pFunc->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
};

void Qtgl::resizeGL(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, (GLint)w, (GLint)h);
  glOrtho(0, 100, 100, 0, -1, 1);
};

void Qtgl::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_QUADS);
  glColor3f(1, 0, 0);
  glVertex2f(10, 90);

  glColor3f(0, 1, 0);
  glVertex2f(90, 90);

  glColor3f(0, 0, 1);
  glVertex2f(90, 10);

  glColor3f(1, 1, 1);
  glVertex2f(10, 10);
  glEnd();
};