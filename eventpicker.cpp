#include "eventpicker.h"
#include "./ui_eventpicker.h"

EventPicker::EventPicker(QListWidget* output, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventPicker)
{
    ui->setupUi(this);
    event_list= output;
}

EventPicker::~EventPicker()
{
    delete ui;
}

void EventPicker::on_btnCancel_clicked()
{
    this->close();
}


void EventPicker::on_trwEvents_itemActivated(QTreeWidgetItem *item, int column)
{
    //Search for duplicates
    QList<QListWidgetItem*> duplist= event_list->findItems(item->text(0), Qt::MatchExactly);
    if (duplist.count() > 0)
    {
        duplist[0]->setSelected(true);
        this->close();
        return;
    }

    QListWidgetItem* newitem= new QListWidgetItem(event_list);
    newitem->setText(item->text(0));
    newitem->setIcon(item->icon(0));
    event_list->addItem(newitem);
    //newitem->setSelected(true);
    this->close();
}

