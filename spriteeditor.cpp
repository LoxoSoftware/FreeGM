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
    this->sprite = sprite;
    ui->setupUi(this);
    ui->btnLoadSprite->setIconSize(ui->btnLoadSprite->size());
    std::cout << "Loading sprite: " << this->sprite->name.toStdString() << std::endl;

    //Loading data from sprite
    ui->txtSprName->setText(this->sprite->name);
    temp_filename= sprite->image_url;
    temp_icon.addFile(temp_filename);
    ui->btnLoadSprite->setIcon(temp_icon);
    ui->spbFrames->setValue(this->sprite->frames);
    ui->chkAnimate->setChecked(this->sprite->animated);

    on_spbFrames_valueChanged(ui->spbFrames->value());
}

SpriteEditor::~SpriteEditor()
{
    delete ui;
}

void SpriteEditor::on_btnOk_clicked()
{
    //Save settings
    sprite->name= ui->txtSprName->text();
    sprite->image_url= temp_filename;
    sprite->animated= ui->chkAnimate->isChecked();
    sprite->frames= ui->spbFrames->value();
    sprite->update();
    this->close();
}

/*void SpriteEditor::on_loadResource(GMSprite* spr)
{
    sprite = spr;
    ui->txtSprName->setText(spr->name);
}*/

void SpriteEditor::on_btnLoadSprite_clicked()
{
    QFileDialog* fd = new QFileDialog();
    fd->setStyleSheet("QPushButton { padding: 8px; padding-left: 20px; padding-right: 20px; }");
    //fd->show();
    fd->setOption(QFileDialog::DontUseNativeDialog, true);
    temp_filename = fd->getOpenFileName(this, "Load a sprite",
                                           QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                           "*.png");
    temp_icon.addFile(temp_filename);
    ui->btnLoadSprite->setIcon(temp_icon);
}


void SpriteEditor::on_spbFrames_valueChanged(int arg1)
{
    ui->scrFrameSel->setVisible(arg1 > 1);
    ui->scrFrameSel->setMaximum(ui->spbFrames->value());
    ui->scrFrameSel->setMinimum(ui->spbFrames->minimum());
}

