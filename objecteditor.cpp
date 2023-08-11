#include "objecteditor.h"
#include "ui_objecteditor.h"

ObjectEditor::ObjectEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ObjectEditor)
{
    ui->setupUi(this);
}

ObjectEditor::~ObjectEditor()
{
    delete ui;
}

void ObjectEditor::on_btnOk_clicked()
{
    this->close();
}

