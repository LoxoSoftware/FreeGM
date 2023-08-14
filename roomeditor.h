#ifndef ROOMEDITOR_H
#define ROOMEDITOR_H

#include "roomview.h"
#include <QMainWindow>

#ifndef RESDEF_HPP
#include "resdef.hpp"
#endif

class GMRoom;
class GMObject;

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
    void on_spbSnapHeight_valueChanged(int arg1);

private slots:
    void on_spbSnapWidth_valueChanged(int arg1);

private slots:
    void on_spbRoomHeight_valueChanged(int arg1);

private slots:
    void on_spbRoomWidth_valueChanged(int arg1);

private slots:
    void on_cmbObjectSel_currentIndexChanged(int index);

private slots:
    void on_btnOk_clicked();

private:
    Ui::RoomEditor *ui;
    GMRoomSettings transport;
    RoomView* roomView;
};

#endif // ROOMEDITOR_H
