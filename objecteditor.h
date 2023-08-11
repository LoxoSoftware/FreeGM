#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include <QMainWindow>

namespace Ui {
class ObjectEditor;
}

class ObjectEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ObjectEditor(QWidget *parent = nullptr);
    ~ObjectEditor();

private slots:
    void on_btnOk_clicked();

private:
    Ui::ObjectEditor *ui;
};

#endif // OBJECTEDITOR_H
