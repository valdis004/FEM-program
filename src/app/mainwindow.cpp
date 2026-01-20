#include "mainwindow.h"
#include "../graphics/qtgl/qtgl.h"

#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFormLayout>
#include <QLabel>
#include <QMdiArea>
#include <QMenuBar>
#include <QObject>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QTextBrowser>
#include <QTextEdit>
#include <QToolBar>
#include <QToolBox>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qdockwidget.h>
#include <qgridlayout.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qthread.h>
#include <qtoolbutton.h>
// #include <stdexcept>

// #include "/home/vladislav/Документы/FEM/FEM program/src/elements/femtypes.h"
// #include "/home/vladislav/Документы/FEM/FEM program/src/mesh/mesh.h"
#include "context-menu/treeContextMenu.h"

MainWindow::MainWindow() {
  setWindowTitle("Fem test");
  resize(1000, 600);
  createMenus();
  createToolBar();
  createLeftDock();
  createToolStrip();
  setupTreeContextMenu(); // Настройка контекстного меню для работы
}

void MainWindow::createLeftDock() {
  // Центральный виджет - текстовый редактор
  QMdiArea *m_pma = new QMdiArea(this);
  m_pma->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_pma->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scene = new Qtgl(this);
  scene->setWindowTitle("Graphic window");
  m_pma->addSubWindow(scene);
  m_pma->setViewMode(QMdiArea::TabbedView);
  m_pma->setTabsClosable(false); // Добавить кнопки закрытия
  m_pma->setTabsMovable(true);   // Возможность перемещать вкладки

  setCentralWidget(m_pma);

  // Док-виджет слева (Дерево)
  QDockWidget *leftDock = new QDockWidget("Tree wiev", this);
  leftDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  treeWidget = new QTreeWidget();
  treeWidget->setHeaderLabel("");
  treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

  QTreeWidgetItem *root =
      new QTreeWidgetItem(treeWidget, QStringList("Plates"));

  connect(treeWidget, &QTreeWidget::customContextMenuRequested, this,
          &MainWindow::onTreeContextMenuRequested);

  leftDock->setWidget(treeWidget);
  leftDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  addDockWidget(Qt::LeftDockWidgetArea, leftDock);
}

void MainWindow::createMenus() {
  QMenuBar *menuBar = this->menuBar();

  QMenu *fileMenu = menuBar->addMenu("Настройки");
  QMenu *viewMenu = menuBar->addMenu("&Вид");
}

void MainWindow::createToolBar() {
  // Заменяем тулбар на виджет с вкладками
  QTabWidget *toolTabs = new QTabWidget();
  toolTabs->setTabPosition(QTabWidget::North);
  toolTabs->setDocumentMode(true);

  // ========== Первая вкладка: Основные операции ==========
  QWidget *mainTab = new QWidget();
  QVBoxLayout *mainLayout = new QVBoxLayout(mainTab);
  mainLayout->setSpacing(2);
  mainLayout->setContentsMargins(2, 2, 2, 2);

  // Первый ряд кнопок
  QHBoxLayout *row1Layout = new QHBoxLayout();
  row1Layout->setSpacing(2);

  QToolButton *newBtn = new QToolButton(mainTab);
  newBtn->setFixedHeight(30);
  newBtn->setAutoRaise(true);
  newBtn->setText("Calculate");
  row1Layout->addWidget(newBtn);
  connect(newBtn, &QToolButton::clicked, this,
          &MainWindow::calculateButtonClicked);

  QPushButton *openBtn =
      new QPushButton(QIcon::fromTheme("document-open"), "Открыть");
  openBtn->setFixedHeight(30);
  row1Layout->addWidget(openBtn);

  QPushButton *saveBtn =
      new QPushButton(QIcon::fromTheme("document-save"), "Сохранить");
  saveBtn->setFixedHeight(30);
  row1Layout->addWidget(saveBtn);

  row1Layout->addSpacing(10); // Небольшой разделитель

  QPushButton *cutBtn =
      new QPushButton(QIcon::fromTheme("edit-cut"), "Вырезать");
  cutBtn->setFixedHeight(30);
  row1Layout->addWidget(cutBtn);

  QPushButton *copyBtn =
      new QPushButton(QIcon::fromTheme("edit-copy"), "Копировать");
  copyBtn->setFixedHeight(30);
  row1Layout->addWidget(copyBtn);

  QPushButton *pasteBtn =
      new QPushButton(QIcon::fromTheme("edit-paste"), "Вставить");
  pasteBtn->setFixedHeight(30);
  row1Layout->addWidget(pasteBtn);

  row1Layout->addStretch();

  // Второй ряд кнопок
  QHBoxLayout *row2Layout = new QHBoxLayout();
  row2Layout->setSpacing(2);

  QPushButton *undoBtn =
      new QPushButton(QIcon::fromTheme("edit-undo"), "Отменить");
  undoBtn->setFixedHeight(30);
  row2Layout->addWidget(undoBtn);

  QPushButton *redoBtn =
      new QPushButton(QIcon::fromTheme("edit-redo"), "Повторить");
  redoBtn->setFixedHeight(30);
  row2Layout->addWidget(redoBtn);

  row2Layout->addStretch();

  mainLayout->addLayout(row1Layout);
  mainLayout->addLayout(row2Layout);

  // ========== Вторая вкладка: Форматирование ==========
  QWidget *resultWindget = new QWidget();
  QGridLayout *structuralForsesLayout = new QGridLayout(resultWindget);
  structuralForsesLayout->setContentsMargins(5, 5, 5, 5);
  structuralForsesLayout->setSpacing(5);
  structuralForsesLayout->setAlignment(Qt::AlignLeft);

  QToolButton *Nx = new QToolButton(resultWindget);
  Nx->setFixedHeight(30);
  Nx->setAutoRaise(true);
  Nx->setText("N_x");

  QToolButton *Ny = new QToolButton(resultWindget);
  Ny->setFixedHeight(30);
  Ny->setAutoRaise(true);
  Ny->setText("N_y");

  QToolButton *Qx = new QToolButton(resultWindget);
  Qx->setFixedHeight(30);
  Qx->setAutoRaise(true);
  Qx->setText("Q_x");

  QToolButton *Qy = new QToolButton(resultWindget);
  Qy->setFixedHeight(30);
  Qy->setAutoRaise(true);
  Qy->setText("Q_y");

  QToolButton *Qxy = new QToolButton(resultWindget);
  Qxy->setFixedHeight(30);
  Qxy->setAutoRaise(true);
  Qxy->setText("Q_xy");

  QToolButton *Mx = new QToolButton(resultWindget);
  Mx->setFixedHeight(30);
  Mx->setAutoRaise(true);
  Mx->setText("M_x");

  QToolButton *My = new QToolButton(resultWindget);
  My->setFixedHeight(30);
  My->setAutoRaise(true);
  My->setText("M_y");

  QToolButton *Mxy = new QToolButton(resultWindget);
  Mxy->setFixedHeight(30);
  Mxy->setAutoRaise(true);
  Mxy->setText("M_xy");

  structuralForsesLayout->addWidget(Nx, 0, 0);
  structuralForsesLayout->addWidget(Ny, 1, 0);
  structuralForsesLayout->addWidget(Qx, 0, 1);
  structuralForsesLayout->addWidget(Qy, 1, 1);
  structuralForsesLayout->addWidget(Qxy, 0, 2);
  structuralForsesLayout->addWidget(Mx, 0, 3);
  structuralForsesLayout->addWidget(My, 1, 3);
  structuralForsesLayout->addWidget(Mxy, 0, 4);

  // Добавляем вкладки
  toolTabs->addTab(mainTab, "Основные");
  toolTabs->addTab(resultWindget, "Результаты");

  // Устанавливаем панель с вкладками в качестве тулбара
  addToolBar(Qt::TopToolBarArea, createToolBarFromWidget(toolTabs));
}

// Вспомогательная функция для создания тулбара из виджета
QToolBar *MainWindow::createToolBarFromWidget(QWidget *widget) {
  QToolBar *toolBar = new QToolBar("Панель вкладок");
  toolBar->setMovable(false); // Можно сделать фиксированной
  toolBar->setAllowedAreas(Qt::TopToolBarArea);
  toolBar->addWidget(widget);
  return toolBar;
}

void MainWindow::createToolStrip() {
  QStatusBar *statusBar = this->statusBar();

  // Статусные метки
  QLabel *statusLabel = new QLabel("Готов");
  statusBar->addWidget(statusLabel);

  QLabel *posLabel = new QLabel("Строка: 1, Колонка: 1");
  statusBar->addPermanentWidget(posLabel);

  QLabel *zoomLabel = new QLabel("100%");
  statusBar->addPermanentWidget(zoomLabel);

  // Прогресс-бар в статус-баре
  QProgressBar *progressBar = new QProgressBar();
  progressBar->setRange(0, 100);
  progressBar->setValue(0);
  progressBar->setFixedWidth(150);
  progressBar->setVisible(false); // По умолчанию скрыт
  statusBar->addPermanentWidget(progressBar);
}

void MainWindow::setupTreeContextMenu() {
  // Создаем структурированное меню
  treeContextMenu = new TreeContextMenu(treeWidget, this);

  // Подключаем сигналы к слотам
  connect(treeContextMenu, &TreeContextMenu::createDefaultPlateScheme, this,
          &MainWindow::createDefaultPlateScheme);
}

void MainWindow::onTreeContextMenuRequested(const QPoint &pos) {
  if (treeContextMenu) {
    treeContextMenu->showMenu(pos);
  }
}

// Функция, отрабатываемая при нажатии кнопки "Create default scheme" У treewiev
// для plate
void MainWindow::createDefaultPlateScheme(QTreeWidgetItem *item) {
  treeContextMenu->createDiologDefualtSchemePlate(this, scene);
}

void MainWindow::calculateButtonClicked() { /*if (scene->)*/ }
