#include "constanteditor.h"
#include "ui_constanteditor.h"

ConstantEditor::ConstantEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConstantEditor)
{
    ui->setupUi(this);
}

ConstantEditor::~ConstantEditor()
{
    delete ui;
}

void ConstantEditor::on_btnOk_clicked()
{
    this->close();
}

