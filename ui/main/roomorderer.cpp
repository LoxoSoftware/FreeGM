#include "roomorderer.h"
#include "ui_roomorderer.h"

RoomOrderer::RoomOrderer(QTreeWidgetItem *folder, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomOrderer)
{
    ui->setupUi(this);

    this->folder= folder;

    //Assemble list
    ui->lstRooms->clear();
    for (int i=0; i<folder->childCount(); i++)
    {
        QListWidgetItem* newitem= new QListWidgetItem(i?QIcon(":/icons/room"):QIcon(":/icons/run")
                                                      ,folder->child(i)->text(0));
        ui->lstRooms->addItem(newitem);
    }
}

RoomOrderer::~RoomOrderer()
{
    delete ui;
}

void RoomOrderer::on_btnCancel_clicked()
{
    this->close();
}

void RoomOrderer::on_btnUp_clicked()
{
    if (ui->lstRooms->selectedItems().count()<=0)
        return;
    if (ui->lstRooms->currentIndex().row()<=0)
        return;
    QString temp= ui->lstRooms->currentItem()->text();
    ui->lstRooms->currentItem()->setText(ui->lstRooms->item(ui->lstRooms->currentIndex().row()-1)->text());
    ui->lstRooms->item(ui->lstRooms->currentIndex().row()-1)->setText(temp);
    ui->lstRooms->setCurrentRow(ui->lstRooms->currentIndex().row()-1);
}

void RoomOrderer::on_btnDown_clicked()
{
    if (ui->lstRooms->selectedItems().count()<=0)
        return;
    if (ui->lstRooms->currentIndex().row()>=ui->lstRooms->count()-1)
        return;
    QString temp= ui->lstRooms->currentItem()->text();
    ui->lstRooms->currentItem()->setText(ui->lstRooms->item(ui->lstRooms->currentIndex().row()+1)->text());
    ui->lstRooms->item(ui->lstRooms->currentIndex().row()+1)->setText(temp);
    ui->lstRooms->setCurrentRow(ui->lstRooms->currentIndex().row()+1);
}

void RoomOrderer::on_btnOk_clicked()
{
    for (int i=0; i<folder->childCount(); i++)
    {
        folder->child(i)->setText(0,ui->lstRooms->item(i)->text());
    }
    this->close();
}

