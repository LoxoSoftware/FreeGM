#include "objecteditor.h"
#include "ui_objecteditor.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QTreeWidgetItem>

#include <iostream>

class GMObject;
extern GMResource* treeitem(QTreeWidgetItem* item);

ObjectEditor::ObjectEditor(GMObject* object, QTreeWidgetItem* sprite_folder, QWidget *parent) :
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
    this->sprite_folder = sprite_folder;
    ui->setupUi(this);
    std::cout << "Loading object: " << this->object->name.toStdString() << std::endl;

    //Assemble sprite sel. combo box
    ui->cmbSpriteSel->addItem("<no sprite>");
    for (int i=0; i<sprite_folder->childCount(); i++)
        ui->cmbSpriteSel->addItem(sprite_folder->child(i)->text(0));

    //Loading data from sprite
    ui->txtObjName->setText(this->object->name);
    if (this->object->image)
        ui->cmbSpriteSel->setCurrentIndex(this->object->image->getFolderIndex()+1);
    else
        ui->cmbSpriteSel->setCurrentIndex(0);
}

ObjectEditor::~ObjectEditor()
{
    delete ui;
}

void ObjectEditor::on_btnOk_clicked()
{
    object->name= ui->txtObjName->text();
    if (ui->cmbSpriteSel->currentIndex() > 0) //If the sprite is set (cmb not set to "<no sprite>")
        object->image= (GMSprite*)treeitem(sprite_folder->child(ui->cmbSpriteSel->currentIndex()-1));
    else
        object->image= nullptr;
    object->visible= ui->chkVisible->isChecked();
    object->update();
    this->close();
}


void ObjectEditor::on_btnAddEvent_clicked()
{
    QInputDialog* idial= new QInputDialog();
    this->setStyleSheet("QPushButton { padding: 6px; padding-left: 20px; padding-right: 20px; }");
    QString itext= idial->getText(this, "Add custom event", "Set condition for custom event");
    this->setStyleSheet("");
    if (itext == "")
        return;

    *events += itext;
    ui->lstEvents->addItem(events->last());
    //ui->lstEvents->item(ui->lstEvents->count()-1)->setIcon(QIcon(":/icons/constant.png"));
}

