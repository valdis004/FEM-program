#pragma once

#include "context-menu/treeContextMenu.h"
#include <QDialogButtonBox>
#include <QMainWindow>
class Qtgl;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  Qtgl *scene;

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
  void calculateButtonClicked();
};
