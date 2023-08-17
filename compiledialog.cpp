#include "compiledialog.h"
#include "ui_compiledialog.h"

CompileDialog::CompileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompileDialog)
{
    ui->setupUi(this);
}

CompileDialog::~CompileDialog()
{
    delete ui;
}

void CompileDialog::on_btnStop_clicked()
{
    this->close();
}

