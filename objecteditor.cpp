#include "objecteditor.h"
#include "ui_objecteditor.h"

#include "eventpicker.h"
#include "vardefiner.h"

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
    transport.variables.clear();
    transport.variables_val.clear();
    transport.variables.append(object->variables);
    transport.variables_val.append(object->variables_val);

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
    object->variables.clear();
    object->variables_val.clear();
    object->variables.append(transport.variables);
    object->variables_val.append(transport.variables_val);
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
}

void ObjectEditor::on_lstEvents_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (ui->lstEvents->currentRow() < 0)
    {
        ui->txtCode->setEnabled(false);
        return;
    }
    ui->txtCode->setEnabled(true);
    ui->txtCode->setReadOnly(false);
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

void ObjectEditor::on_btnRemEvent_clicked()
{
    if (ui->lstEvents->selectedItems().count() <= 0)
        return;
    QMessageBox::StandardButton reply;
    reply= QMessageBox::question(this,"Confirm event deletion","Do you really want to delete this event? All its code will be lost!", QMessageBox::Yes|QMessageBox::No);
    if (reply != QMessageBox::Yes)
        return;
    int index= ui->lstEvents->currentIndex().row();
    event_code.removeAt(index);
    //events.removeAt(index);
    ui->lstEvents->takeItem(index);
}


void ObjectEditor::on_btnVariables_clicked()
{
    VarDefiner* vdial= new VarDefiner(&transport, mainwindow);
    vdial->setModal(true);
    vdial->open();
}

