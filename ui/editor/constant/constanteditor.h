#ifndef CONSTANTEDITOR_H
#define CONSTANTEDITOR_H

#include <QMainWindow>

#include "../../../resdef.hpp"

namespace Ui {
class ConstantEditor;
}

class ConstantEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConstantEditor(GMConstant* cc, QWidget *parent = nullptr);
    ~ConstantEditor();

private slots:
    void on_btnOk_clicked();

private:
    Ui::ConstantEditor *ui;
    GMConstant* cc;
};

#endif // CONSTANTEDITOR_H
