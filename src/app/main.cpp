#include "/home/vladislav/Документы/FEM/FEM program/src/elements/elementprovider.h"
#include "mainwindow.h"
#include <QApplication>
// #include <glfw.h>

int main(int argc, char *argv[]) {
  ElementProvider::init();

  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}
