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

void CompileDialog::console_write(QString str)
{
    ui->txtConsole->appendPlainText(str);
}

void CompileDialog::console_clear()
{
    ui->txtConsole->clear();
}

void CompileDialog::set_progress(int perc)
{
    ui->prbProgress->setValue(perc);
    if (perc == 100)
        ui->btnStop->setEnabled(false);
    else
        ui->btnStop->setEnabled(true);
}
