#pragma once

#include "context-menu/treeContextMenu.h"
#include "femtypes.h"
#include <QDialogButtonBox>
#include <QMainWindow>
#include <qglobal.h>
#include <qmdiarea.h>
#include <qstandarditemmodel.h>
#include <qtableview.h>
#include <qtoolbutton.h>

class mesh;
class Qtgl;
class Solver;
class QTableView;
class QStandardItemModel;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  QVector<shared_ptr<AbstractElement>> elements;

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
  // Result table vars
  QStandardItemModel *model{nullptr};
  QTableView *resultsView{nullptr};
  shared_ptr<AbstractElement> selectedELement{nullptr};

  void createLeftDock();
  void createMenus();
  void createToolBar();
  void createToolStrip();
  QToolBar *createToolBarFromWidget(QWidget *widget);
  QTreeWidget *treeWidget;
  TreeContextMenu *treeContextMenu;
  void setupTreeContextMenu();
  void getResultTable(shared_ptr<AbstractElement> selectedELement,
                      int selectedId, QStandardItemModel *model);
  void setSpanResultTable(shared_ptr<AbstractElement> selectedELement,
                          QTableView *resultsView);

private slots:
  void onTreeContextMenuRequested(const QPoint &pos);
  void createDefaultPlateScheme(QTreeWidgetItem *item);
  void calculateButtonClicked();
  void createTableResultsTab();

  // Result table
  void updateResultTable();
};
