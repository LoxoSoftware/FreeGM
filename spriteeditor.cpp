#include "spriteeditor.h"
#include "ui_spriteeditor.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QErrorMessage>

#include <iostream>

SpriteEditor::SpriteEditor(GMSprite* sprite, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpriteEditor)
{
    if (!sprite)
    {
        QErrorMessage* msg = new QErrorMessage();
        msg->showMessage("Invalid input sprite");
        this->close();
        return;
    }
    ui->setupUi(this);
    ui->txtSprName->setText(this->sprite->name);
}

SpriteEditor::~SpriteEditor()
{
    delete ui;
}

void SpriteEditor::on_btnOk_clicked()
{
    this->close();
}

/*void SpriteEditor::on_loadResource(GMSprite* spr)
{
    sprite = spr;
    ui->txtSprName->setText(spr->name);
}*/

void SpriteEditor::on_btnLoadSprite_clicked()
{
    ui->btnLoadSprite->setIconSize(ui->btnLoadSprite->size());

    QFileDialog* fd = new QFileDialog();
    fd->setStyleSheet("QPushButton { padding: 8px; padding-left: 20px; padding-right: 20px; }");
    //fd->show();
    fd->setOption(QFileDialog::DontUseNativeDialog, true);
    QString filename = fd->getOpenFileName(this, "Load a sprite",
                                           QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                           "*.png");
    QIcon newico = *new QIcon(filename);

    ui->btnLoadSprite->setIcon(newico);
}

