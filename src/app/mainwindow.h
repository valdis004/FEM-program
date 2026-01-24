#pragma once

#include "context-menu/treeContextMenu.h"
#include <QDialogButtonBox>
#include <QMainWindow>
#include <qglobal.h>
#include <qmdiarea.h>
#include <qtableview.h>
#include <qtoolbutton.h>

class mesh;
class Qtgl;
class Solver;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  QMdiArea *m_pma;
  Qtgl *scene{nullptr};
  Mesh *mesh{nullptr};
  Solver *solver{nullptr};
  QVector<QToolButton *> resultButtons;
  QLabel *statusLabel;
  QToolButton *tablesBtn;

  ~MainWindow();

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
  void createTableResultsTab();
};
