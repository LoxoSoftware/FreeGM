#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#ifndef RESDEF_HPP
#include "resdef.hpp"
#endif

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
    void on_btnAddEvent_clicked();

private slots:
    void on_btnOk_clicked();

private:
    Ui::ObjectEditor *ui;
    GMObject* object;
    QList<QString>* events= new QList<QString>;
    QTreeWidgetItem* sprite_folder;
};

#endif // OBJECTEDITOR_H
