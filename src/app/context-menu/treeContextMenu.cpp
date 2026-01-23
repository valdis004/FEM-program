#include "treeContextMenu.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/graphics/qtgl/qtgl.h"
#include "/home/vladislav/Документы/FEM/FEM program/src/mesh/mesh.h"
#include <QContextMenuEvent>
#include <QDialog>
#include <QMessageBox>
#include <QThread>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qdialogbuttonbox.h>
#include <qformlayout.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <vector>

#define TEXT_PLATE_STANDART_SCHEME "Create default scheme"

TreeContextMenu::TreeContextMenu(QTreeWidget *treeWidget, QObject *parent)
    : QObject(parent), treeWidget(treeWidget), currentItem(nullptr) {

  // Создаем меню
  contextMenu = new QMenu();

  // Настраиваем стандартное меню
  setupMenu();
  connectActions();
}

TreeContextMenu::~TreeContextMenu() { delete contextMenu; }

void TreeContextMenu::setupMenu() {
  // Стандартные действия
  addAction(TEXT_PLATE_STANDART_SCHEME);
}

void TreeContextMenu::showMenu(const QPoint &pos) {
  // Определяем элемент под курсором
  currentItem = treeWidget->itemAt(pos);

  // в зависимости от выбранного элемента
  if (actions.contains("delete")) {
    bool canDelete = currentItem && currentItem->parent();
    actions["delete"]->setEnabled(canDelete);
  }

  if (actions.contains("rename")) {
    actions["rename"]->setEnabled(currentItem != nullptr);
  }

  // Показываем меню
  if (contextMenu) {
    contextMenu->exec(treeWidget->viewport()->mapToGlobal(pos));
  }
}

void TreeContextMenu::connectActions() {
  // Подключаем все действия к одному слоту
  for (QAction *action : contextMenu->actions()) {
    connect(action, &QAction::triggered, this,
            &TreeContextMenu::onActionTriggered);
  }
}

void TreeContextMenu::addAction(const QString &text, const QIcon &icon) {
  QAction *action = contextMenu->addAction(icon, text);
  action->setData(text); // Сохраняем имя действия
  actions[text] = action;
}

void TreeContextMenu::onActionTriggered() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (!action)
    return;

  QString actionName = action->data().toString();

  // Испускаем соответствующие сигналы
  if (actionName == TEXT_PLATE_STANDART_SCHEME) {
    emit createDefaultPlateScheme(currentItem);
  }

  // Общий сигнал
  emit actionTriggered(actionName, currentItem);
}

void TreeContextMenu::createDiologDefualtSchemePlate(QWidget *mainWindow,
                                                     Qtgl *scene, Mesh *&mesh) {
  QDialog *d = new QDialog(mainWindow);
  d->setFixedSize({500, 260});
  d->setWindowTitle("Settings of scheme");
  // d->setModal(true);
  // d->setWindowFlag(Qt);
  // d->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint |
  // Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

  QVBoxLayout *mainLayout = new QVBoxLayout(d);
  QFormLayout *formLayout = new QFormLayout();
  QComboBox *comboBox = new QComboBox(d);
  comboBox->addItem("MITC4");
  comboBox->addItem("MITC9");
  comboBox->addItem("MITC16");
  formLayout->addRow("Тип элемента:", comboBox);

  // Выравнивание
  formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
  formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
  formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);

  d->setSizeGripEnabled(true); // Добавляет маркер изменения размера в углу

  // 2. Создаем панель с кнопками
  QDialogButtonBox *buttonBox = new QDialogButtonBox(d);
  // Добавляем стандартные кнопки
  buttonBox->setStandardButtons(QDialogButtonBox::Ok |
                                QDialogButtonBox::Cancel);
  // Подключаем сигналы: нажатие OK/Отмена закроет диалог с соответствующим
  // результатом
  connect(buttonBox, &QDialogButtonBox::accepted, d, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, d, &QDialog::reject);

  // 3. Задаем основную компоновку
  mainLayout->addLayout(formLayout);
  mainLayout->addWidget(buttonBox); // Панель кнопок — внизу
  // d->setLayout(mainLayout);
  d->show();

  ElementType type = (ElementType)comboBox->currentIndex();

  if (d->exec() == QDialog::Accepted) {
    QMessageBox *mes = new QMessageBox(mainWindow);
    mes->setWindowTitle("Generating default mesh...");
    mes->show();

    mesh = new Mesh();

    connect(mesh, &Mesh::progressChanged, this,
            &TreeContextMenu::updateProgress);
    connect(mesh, &Mesh::meshFinished, this, &TreeContextMenu::showResult);

    // Создаем отдельный поток для mesh_
    QThread *workerThread = new QThread();
    mesh->moveToThread(workerThread); // mesh_ теперь принадлежит workerThread

    // Когда поток запустится, выполним создание меша
    connect(workerThread, &QThread::started, this, [=]() {
      mesh->createDefaultMesh(type, mes);

      // Копируем данные в локальные переменные
      QVector<Node *> nodes = mesh->nodes;
      QVector<AbstractFemElement *> elements = mesh->elements;

      // Передаем в основной поток
      QMetaObject::invokeMethod(
          this,
          [=]() {
            scene->setMeshData(nodes, elements);
            mes->accept();
            mes->deleteLater();

            // Завершаем поток
            workerThread->quit();
          },
          Qt::QueuedConnection);
    });

    // Удаляем поток и mesh_ при завершении
    connect(workerThread, &QThread::finished, workerThread,
            &QThread::deleteLater);
    // connect(workerThread, &QThread::finished, mesh_, &Mesh::deleteLater);

    workerThread->start();
  }
}

void TreeContextMenu::updateProgress(QMessageBox *mes, int count) {
  mes->setText(QString("Creating element: %1").arg(count));
}

void TreeContextMenu::showResult(QMessageBox *mes, int count) {
  mes->setText(QString("Sucsesfully Created %1 elements").arg(count));
}
