#pragma once

#include "context-menu/treeContextMenu.h"
#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

private:
  void createLeftDock();
  void createMenus();
  void createToolBar();
  void createToolStrip();
  QToolBar *createToolBarFromWidget(QWidget *widget);
  QTreeWidget *treeWidget;
  TreeContextMenu *treeContextMenu;
  void setupTreeContextMenu();

private slots:
  void onTreeContextMenuRequested(const QPoint &pos);
  void createDefaultPlateScheme(QTreeWidgetItem *item);
};
