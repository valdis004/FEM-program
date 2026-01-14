#include "qtgl.h"
#include <QOpenGLFunctions>
#include <QtGui>
#include <qmatrix4x4.h>
#include <qnamespace.h>
#include <qpoint.h>

Qtgl::Qtgl(QWidget *pwgt /*= 0*/)
    : QOpenGLWidget(pwgt), m_xRotate(0), m_yRotate(0) {}

// ----------------------------------------------------------------------
/*virtual*/ void Qtgl::initializeGL() {
  QOpenGLFunctions *pFunc = QOpenGLContext::currentContext()->functions();
  pFunc->glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

  pFunc->glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  m_nPyramid = createPyramid(0.9f);
}

// ----------------------------------------------------------------------
/*virtual*/ void Qtgl::resizeGL(int nWidth, int nHeight) {
  glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
}

// ----------------------------------------------------------------------
/*virtual*/ void Qtgl::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -4.0 * m_scale);

  glRotatef(m_xRotate, 1.0, 0.0, 0.0);
  glRotatef(m_yRotate, 0.0, 1.0, 0.0);

  glCallList(m_nPyramid);
}

// ----------------------------------------------------------------------
/*virtual*/ void Qtgl::mousePressEvent(QMouseEvent *pe) {
  if (pe->button() != Qt::LeftButton) {
    isLeftBut = false;
    return;
  }

  isLeftBut = true;
  m_ptPosition = pe->pos();
}

void Qtgl::wheelEvent(QWheelEvent *pe) {
  GLfloat delta = pe->angleDelta().y();
  if (delta > 0)
    m_scale /= 1.1f;
  else
    m_scale *= 1.1f;
  update();
}

// ----------------------------------------------------------------------
/*virtual*/ void Qtgl::mouseMoveEvent(QMouseEvent *pe) {
  if (!isLeftBut)
    return;

  m_xRotate += 180 * (GLfloat)(pe->y() - m_ptPosition.y()) / height();
  m_yRotate += 180 * (GLfloat)(pe->x() - m_ptPosition.x()) / width();
  update();

  m_ptPosition = pe->pos();
}

// ----------------------------------------------------------------------
GLuint Qtgl::createPyramid(GLfloat fSize /*=1.0f*/) {
  GLuint n = glGenLists(1);

  glNewList(n, GL_COMPILE);
  glBegin(GL_TRIANGLE_FAN);
  glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
  glVertex3f(0.0, fSize, 0.0);
  glVertex3f(-fSize, -fSize, fSize);
  glVertex3f(fSize, -fSize, fSize);
  glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
  glVertex3f(fSize, -fSize, -fSize);
  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
  glVertex3f(-fSize, -fSize, -fSize);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glVertex3f(-fSize, -fSize, fSize);
  glEnd();

  glBegin(GL_QUADS);
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  glVertex3f(-fSize, -fSize, fSize);
  glVertex3f(fSize, -fSize, fSize);
  glVertex3f(fSize, -fSize, -fSize);
  glVertex3f(-fSize, -fSize, -fSize);
  glEnd();
  glEndList();

  return n;
}