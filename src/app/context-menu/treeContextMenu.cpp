#include "treeContextMenu.h"
#include <QContextMenuEvent>
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
