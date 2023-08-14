#include "roomeditor.h"
#include "ui_roomeditor.h"
#include "roomview.h"

#include <QBrush>
#include <QColor>
#include <QGraphicsView>
#include <QGraphicsItem>

QMainWindow* mainwindow;

extern QTreeWidgetItem* folder_objects;
extern GMResource* treeitem(QTreeWidgetItem* item);

RoomEditor::RoomEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoomEditor)
{
    ui->setupUi(this);

    transport.width = ui->spbRoomWidth->value();
    transport.height = ui->spbRoomHeight->value();
    transport.snapX = ui->spbSnapWidth->value();
    transport.snapY = ui->spbSnapHeight->value();
    transport.selected_object = nullptr;

    roomView = new RoomView(&transport, ui->centralwidget);

    //Assemble object sel. combo box
    ui->cmbObjectSel->addItem("<nothing>");
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        if(((GMObject*)treeitem(folder_objects->child(i)))->image)
            ui->cmbObjectSel->addItem(((GMObject*)treeitem(folder_objects->child(i)))->image->icon, folder_objects->child(i)->text(0));
        else //If the object does not have a sprite associated to it
            ui->cmbObjectSel->addItem(QIcon(":/icons/object"), folder_objects->child(i)->text(0));
    }

    roomView->setMaximumSize(ui->spbRoomWidth->value(), ui->spbRoomHeight->value());
    roomView->setMinimumSize(roomView->maximumSize());

    roomView->show();
}

RoomEditor::~RoomEditor()
{
    delete ui;
}

void RoomEditor::on_btnOk_clicked()
{
    this->close();
}

void RoomEditor::on_cmbObjectSel_currentIndexChanged(int index)
{
    if (index == 0) //Selected '<nothing>'
        transport.selected_object= nullptr;
    else
        transport.selected_object= (GMObject*)treeitem(folder_objects->child(index-1));
}


void RoomEditor::on_spbRoomWidth_valueChanged(int arg1)
{
    transport.width= arg1;
    roomView->redraw();
}


void RoomEditor::on_spbRoomHeight_valueChanged(int arg1)
{
    transport.height= arg1;
    roomView->redraw();
}

void RoomEditor::on_spbSnapWidth_valueChanged(int arg1)
{
    transport.snapX= arg1;
    roomView->redraw();
}

void RoomEditor::on_spbSnapHeight_valueChanged(int arg1)
{
    transport.snapY= arg1;
    roomView->redraw();
}
