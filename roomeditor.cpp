#include "roomeditor.h"
#include "ui_roomeditor.h"
#include "roomview.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QColorDialog>

#include <iostream>

extern QMainWindow* mainwindow;
extern QTreeWidgetItem* folder_objects;
extern GMResource* resource_find(QTreeWidgetItem* item);

RoomEditor::RoomEditor(GMRoom* room, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoomEditor)
{
    transport.target_room= room;

    ui->setupUi(this);

    disconnectControls= true;

    ui->dckTop->setTitleBarWidget(new QWidget(ui->dckTop));

    //Load data from room
    ui->txtRoomName->setText(room->name);
    transport.instances= room->instances;
    ui->spbRoomWidth->setValue(room->room_width);
    ui->spbRoomHeight->setValue(room->room_height);
    ui->spbSnapWidth->setValue(room->room_snapX);
    ui->spbSnapHeight->setValue(room->room_snapY);
    ui->chkGrid->setChecked(room->room_grid);
    ui->btnVoidColor->setStyleSheet("background-color: "+room->back_color.name()+";");
    ui->chkFillVoid->setChecked(room->fill_back);

    transport.width = ui->spbRoomWidth->value();
    transport.height = ui->spbRoomHeight->value();
    transport.snapX = ui->spbSnapWidth->value();
    transport.snapY = ui->spbSnapHeight->value();
    transport.drawGrid = ui->chkGrid;
    transport.selected_object = nullptr;
    transport.back_color = room->back_color;
    transport.fill_back = ui->chkFillVoid->isChecked();

    roomView = new RoomView(&transport, ui->centralwidget);

    //Assemble object sel. combo box
    ui->cmbObjectSel->addItem("<nothing>");
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        if(((GMObject*)resource_find(folder_objects->child(i)))->image)
            ui->cmbObjectSel->addItem(((GMObject*)resource_find(folder_objects->child(i)))->image->icon, folder_objects->child(i)->text(0));
        else //If the object does not have a sprite associated to it
            ui->cmbObjectSel->addItem(QIcon(":/icons/object"), folder_objects->child(i)->text(0));
    }

    roomView->setMaximumSize(ui->spbRoomWidth->value(), ui->spbRoomHeight->value());
    roomView->setMinimumSize(roomView->maximumSize());

    roomView->show();

    disconnectControls= false;
}

RoomEditor::~RoomEditor()
{
    delete ui;
}

void RoomEditor::on_btnOk_clicked()
{
    //Save data to room
    transport.target_room->name= ui->txtRoomName->text();
    transport.target_room->instances= transport.instances;
    transport.target_room->room_width= ui->spbRoomWidth->value();
    transport.target_room->room_height= ui->spbRoomHeight->value();
    transport.target_room->room_snapX= ui->spbSnapWidth->value();
    transport.target_room->room_snapY= ui->spbSnapHeight->value();
    transport.target_room->room_grid= ui->chkGrid->isChecked();
    transport.target_room->back_color= transport.back_color;
    transport.target_room->fill_back= ui->chkFillVoid->isChecked();
    transport.target_room->update();
    this->close();
}

void RoomEditor::on_cmbObjectSel_currentIndexChanged(int index)
{
    if (index == 0) //Selected '<nothing>'
        transport.selected_object= nullptr;
    else
        transport.selected_object= (GMObject*)resource_find(folder_objects->child(index-1));
}


void RoomEditor::on_spbRoomWidth_valueChanged(int arg1)
{
    if (disconnectControls)
        return;
    transport.width= arg1;
    roomView->redraw();
}


void RoomEditor::on_spbRoomHeight_valueChanged(int arg1)
{
    if (disconnectControls)
        return;
    transport.height= arg1;
    roomView->redraw();
}

void RoomEditor::on_spbSnapWidth_valueChanged(int arg1)
{
    if (disconnectControls)
        return;
    transport.snapX= arg1;
    roomView->redraw();
}

void RoomEditor::on_spbSnapHeight_valueChanged(int arg1)
{
    if (disconnectControls)
        return;
    transport.snapY= arg1;
    roomView->redraw();
}

void RoomEditor::on_chkGrid_stateChanged(int arg1)
{
    if (disconnectControls)
        return;
    transport.drawGrid = ui->chkGrid->isChecked();
    roomView->redraw();
}


void RoomEditor::on_btnVoidColor_clicked()
{
    QColorDialog cdial= QColorDialog(transport.back_color);
    transport.back_color= cdial.getColor();
    ui->btnVoidColor->setStyleSheet("background-color: "+transport.back_color.name()+";");
    roomView->redraw();
}


void RoomEditor::on_chkFillVoid_stateChanged(int arg1)
{
    if (disconnectControls)
        return;
    transport.fill_back = ui->chkFillVoid->isChecked();
    roomView->redraw();
}

