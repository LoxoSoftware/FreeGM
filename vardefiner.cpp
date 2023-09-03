#include "vardefiner.h"
#include "ui_vardefiner.h"

#include <QTableWidget>

VarDefiner::VarDefiner(GMObjectVars* transport, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VarDefiner)
{
    ui->setupUi(this);

    this->transport= transport;

    for(int i=transport->variables.count()-1; i>=0; i--)
    {
        ui->tblVars->insertRow(0);
        ui->tblVars->setItem(0,0,new QTableWidgetItem(transport->variables[i]));
        ui->tblVars->setItem(0,1,new QTableWidgetItem(transport->variables_val[i]));
    }
}

VarDefiner::~VarDefiner()
{
    delete ui;
}

void VarDefiner::on_btnOk_clicked()
{
    transport->variables.clear();
    transport->variables_val.clear();
    for (int i=0; i<ui->tblVars->rowCount(); i++)
    {
        if (ui->tblVars->item(i,0)->text() == "") continue;
        transport->variables+= ui->tblVars->item(i,0)->text();
        transport->variables_val+= ui->tblVars->item(i,1)->text();
    }
    this->close();
}

void VarDefiner::on_btnAdd_clicked()
{
    ui->tblVars->insertRow(0);
    ui->tblVars->setItem(0,0,new QTableWidgetItem());
    ui->tblVars->setItem(0,1,new QTableWidgetItem());
    ui->tblVars->editItem(ui->tblVars->item(0,0));
}

