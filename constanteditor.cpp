#include "constanteditor.h"
#include "ui_constanteditor.h"

#include <QTreeWidgetItem>

class GMConstant;
class GMResource;
extern GMResource* treeitem(QTreeWidgetItem* item);

ConstantEditor::ConstantEditor(GMConstant* cc, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConstantEditor)
{
    if (!cc)
    {
        throw "Invalid input constant";
        this->close();
        return;
    }
    this->cc= cc;
    ui->setupUi(this);
    ui->txtCname->setText(cc->name);
    ui->txtCvalue->setText(cc->value);
}

ConstantEditor::~ConstantEditor()
{
    delete ui;
}

void ConstantEditor::on_btnOk_clicked()
{
    cc->name= ui->txtCname->text();
    cc->value= ui->txtCvalue->text();
    cc->update();
    this->close();
}

