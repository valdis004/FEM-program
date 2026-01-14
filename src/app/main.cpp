#include "mainwindow.h"
#include <QApplication>
// #include <glfw.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return app.exec();
}
