#include "spriteeditor.h"
#include "ui_spriteeditor.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QErrorMessage>
#include <QTimerEvent>

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
    temp_image= this->sprite->image;
    temp_icon= this->sprite->icon;
    ui->btnLoadSprite->setIcon(temp_icon);
    ui->spbFrames->setValue(this->sprite->frames);
    ui->chkAnimate->setChecked(this->sprite->animated);

    on_spbFrames_valueChanged(ui->spbFrames->value());
    on_chkAnimate_stateChanged(0);
}

SpriteEditor::~SpriteEditor()
{
    delete ui;
}

void SpriteEditor::on_btnOk_clicked()
{
    //Save settings
    sprite->name= ui->txtSprName->text();
    sprite->icon= temp_icon;
    sprite->image= temp_image;
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
    QString temp_filename = fd->getOpenFileName(this, "Load a sprite",
                                           ".",
                                           "Supported image format (*.png *.jpg *.jpeg *.bmp *.svg)");
    temp_image.load(temp_filename);
    temp_icon= QIcon(QPixmap::fromImage(temp_image));
    ui->btnLoadSprite->setIcon(temp_icon);
}

void SpriteEditor::on_spbFrames_valueChanged(int arg1)
{
    ui->scrFrameSel->setVisible(arg1 > 1 && !ui->chkAnimate->isChecked());
    ui->scrFrameSel->setMaximum(ui->spbFrames->value());
    ui->scrFrameSel->setMinimum(ui->spbFrames->minimum());
    //Crop the image
    temp_icon.detach();
    QPixmap newimg= QPixmap::fromImage(temp_image);
    temp_icon= QIcon(newimg.copy((temp_image.width()/arg1)*0,0,
                                  temp_image.width()/arg1,temp_image.height()));
    ui->scrFrameSel->setValue(0);
    ui->btnLoadSprite->setIcon(temp_icon);
}

void SpriteEditor::on_scrFrameSel_valueChanged(int value)
{
    //Crop the image
    temp_icon.detach();
    QPixmap newimg= QPixmap::fromImage(temp_image);
    temp_icon= QIcon(newimg.copy((temp_image.width()/ui->spbFrames->value())*(value-1),0,
                                  temp_image.width()/ui->spbFrames->value(),temp_image.height()));
    ui->btnLoadSprite->setIcon(temp_icon);
}

void SpriteEditor::timerEvent(QTimerEvent* event)
{
    if (ui->scrFrameSel->value() >= ui->scrFrameSel->maximum())
        ui->scrFrameSel->setValue(ui->scrFrameSel->minimum());
    else
        ui->scrFrameSel->setValue(ui->scrFrameSel->value()+1);
}

void SpriteEditor::on_chkAnimate_stateChanged(int arg1)
{
    if (ui->chkAnimate->isChecked())
    {
        timer_id= this->startTimer(100);
        ui->scrFrameSel->setVisible(false);
    }
    else
    {
        this->killTimer(timer_id);
        ui->scrFrameSel->setVisible(ui->spbFrames->value()>1);
    }
}

