#ifndef ROOMEDITOR_H
#define ROOMEDITOR_H

#include <QMainWindow>

namespace Ui {
class RoomEditor;
}

class RoomEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoomEditor(QWidget *parent = nullptr);
    ~RoomEditor();

private slots:
    void on_btnOk_clicked();

private:
    Ui::RoomEditor *ui;
};

#endif // ROOMEDITOR_H
