#include "objecteditor.h"
#include "ui_objecteditor.h"

#include "eventpicker.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QTreeWidgetItem>

#include <iostream>

class GMObject;
extern GMResource* resource_find(QTreeWidgetItem* item);
extern QTreeWidgetItem* folder_sprites;

ObjectEditor::ObjectEditor(GMObject* object, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ObjectEditor)
{
    if (!object)
    {
        throw "Invalid input object";
        this->close();
        return;
    }
    this->object = object;
    this->sprite_folder = folder_sprites;
    ui->setupUi(this);
    std::cout << "Loading object: " << this->object->name.toStdString() << std::endl;

    //Assemble sprite sel. combo box
    ui->cmbSpriteSel->addItem("<no sprite>");
    for (int i=0; i<sprite_folder->childCount(); i++)
        ui->cmbSpriteSel->addItem(((GMSprite*)resource_find(sprite_folder->child(i)))->icon, sprite_folder->child(i)->text(0));

    //Loading data from object
    ui->txtObjName->setText(this->object->name);
    if (this->object->image)
        ui->cmbSpriteSel->setCurrentIndex(this->object->image->getFolderIndex()+1);
    else
        ui->cmbSpriteSel->setCurrentIndex(0);
    this->events.clear();
    this->event_code.clear();
    for (int i=0; i<256; i++)
        this->event_code += ""; //Allocate space for up to X event_codes (builtin and user)
    for (int i=0; i<object->events.count(); i++)
    {
        this->events += object->events[i];
        this->event_code[i] = object->event_code[i];
        ui->lstEvents->addItem(&this->events.last());
    }

    //Disable the code editor (until a trigger is selected)
    ui->txtCode->setEnabled(false);
    ui->txtCode->setText("<-- Please select an event by clicking on its trigger or create a new one");
}

ObjectEditor::~ObjectEditor()
{
    ui->lstEvents->setCurrentRow(-1);
    delete ui;
    //Hopefully prevent memory leaks
    this->events.clear();
    this->event_code.clear();
}

void ObjectEditor::on_btnOk_clicked()
{
    object->name= ui->txtObjName->text();
    if (ui->cmbSpriteSel->currentIndex() > 0) //If the sprite is set (cmb not set to "<no sprite>")
        object->image= (GMSprite*)resource_find(sprite_folder->child(ui->cmbSpriteSel->currentIndex()-1));
    else
        object->image= nullptr;
    object->visible= ui->chkVisible->isChecked();
    object->events.clear();
    for (int i=0; i<ui->lstEvents->count(); i++)
        object->events += *ui->lstEvents->item(i);
    object->event_code.clear();
    std::cout << "Saving " << ui->lstEvents->count() << " events..." << std::endl;
    for (int i=0; i<ui->lstEvents->count(); i++)
    {
        std::cout << object->event_code.count() << std::endl;
        object->event_code += this->event_code[i];
    }
    std::cout << "Saved object details!" << std::endl;
    object->update();
    this->close();
}


void ObjectEditor::on_btnAddEvent_clicked()
{
    QDialog* idial= new EventPicker(ui->lstEvents);
    idial->setParent(mainwindow, Qt::Dialog);
    idial->setModal(true);
    idial->open();
    //this->setStyleSheet("QPushButton { padding: 6px; padding-left: 20px; padding-right: 20px; }");
    //QString itext= idial->getText(this, "Add custom event", "Set condition for custom event");
    //this->setStyleSheet("");
    //if (itext == "")
        //return;

    //*events += itext;
    //ui->lstEvents->addItem(events->last());
    //ui->lstEvents->item(ui->lstEvents->count()-1)->setIcon(QIcon(":/icons/constant.png"));
}

void ObjectEditor::on_lstEvents_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (ui->lstEvents->currentRow() < 0)
    {
        ui->txtCode->setEnabled(false);
        return;
    }
    ui->txtCode->setEnabled(true);
    ui->txtCode->setText(event_code[ui->lstEvents->currentRow()]);
}
void ObjectEditor::on_lstEvents_itemClicked(QListWidgetItem *item)
{
    ObjectEditor::on_lstEvents_currentItemChanged(item, nullptr);
}


void ObjectEditor::on_txtCode_textChanged()
{
    if (ui->lstEvents->currentRow() < 0)
    {
        ui->txtCode->setEnabled(false);
        return;
    }
    event_code[ui->lstEvents->currentRow()]= ui->txtCode->toPlainText();
}

