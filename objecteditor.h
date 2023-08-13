#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

#ifndef RESDEF_HPP
#include "resdef.hpp"
#endif

extern QWidget* mainwindow;

namespace Ui {
class ObjectEditor;
}

class ObjectEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ObjectEditor(GMObject* object, QTreeWidgetItem* sprite_folder, QWidget *parent = nullptr);
    ~ObjectEditor();

private slots:
    void on_txtCode_textChanged();
    void on_lstEvents_itemClicked(QListWidgetItem *item);
    void on_lstEvents_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_btnAddEvent_clicked();
    void on_btnOk_clicked();

private:
    Ui::ObjectEditor *ui;
    GMObject* object;
    QList<QListWidgetItem> events;
    QList<QString> event_code;
    QTreeWidgetItem* sprite_folder;
};

#endif // OBJECTEDITOR_H
