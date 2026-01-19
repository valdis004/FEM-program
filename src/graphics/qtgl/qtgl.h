#pragma once

#include "/home/vladislav/Документы/FEM/FEM program/src/elements/element.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/elements/node.h"
#include <QOpenGLWidget>
#include <qevent.h>

class Qtgl : public QOpenGLWidget {
private:
  GLuint m_nMesh;
  GLfloat m_xRotate;
  GLfloat m_yRotate;
  GLfloat m_scale = 1.0f;
  QPoint m_ptPosition;
  bool isLeftBut = false;

  std::vector<Node *> m_nodes;               // Храним узлы
  std::vector<AbstractElement *> m_elements; // Храним элементы
  bool m_meshDataValid = false;              // Флаг наличия данных
  float m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
  float m_centerX, m_centerY, m_centerZ, m_scaleFactor;

  void normalizeMeshData();   // Нормировка данных сетки
  void calculateMeshBounds(); // Вычисление границ
  void createMeshDisplayList();

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
  void setMeshData(const std::vector<Node *> &nodes,
                   const std::vector<AbstractElement *> &elements);
};
