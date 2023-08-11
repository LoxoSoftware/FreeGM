#ifndef CONSTANTEDITOR_H
#define CONSTANTEDITOR_H

#include <QMainWindow>

namespace Ui {
class ConstantEditor;
}

class ConstantEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConstantEditor(QWidget *parent = nullptr);
    ~ConstantEditor();

private slots:
    void on_btnOk_clicked();

private:
    Ui::ConstantEditor *ui;
};

#endif // CONSTANTEDITOR_H
