#ifndef ROOMEDITOR_H
#define ROOMEDITOR_H

#include "roomview.h"
#include <QMainWindow>

#ifndef RESDEF_HPP
#include "../../../resdef.hpp"
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
    explicit RoomEditor(GMRoom* room, QWidget *parent = nullptr);
    ~RoomEditor();

private slots:
    void on_spbSnapHeight_valueChanged(int arg1);

    void on_spbSnapWidth_valueChanged(int arg1);

    void on_spbRoomHeight_valueChanged(int arg1);

    void on_spbRoomWidth_valueChanged(int arg1);

    void on_cmbObjectSel_currentIndexChanged(int index);

    void on_btnOk_clicked();

    void on_chkGrid_stateChanged(int arg1);

    void on_btnVoidColor_clicked();

    void on_chkFillVoid_stateChanged(int arg1);

private:
    Ui::RoomEditor *ui;
    GMRoomSettings transport;
    RoomView* roomView;
    bool disconnectControls= true;
};

#endif // ROOMEDITOR_H
