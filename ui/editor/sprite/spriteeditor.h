#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#ifndef RESDEF_HPP
#include "../../../resdef.hpp"
#endif

class GMSprite;
GMResource* resource_find(QString name);

namespace Ui {
class SpriteEditor;
}

class SpriteEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpriteEditor(GMSprite* sprite, QWidget *parent = nullptr);
    ~SpriteEditor();

private slots:
    void on_chkAnimate_stateChanged(int arg1);

private slots:
    void on_scrFrameSel_valueChanged(int value);
    void on_spbFrames_valueChanged(int arg1);
    void on_btnLoadSprite_clicked();
    void on_btnOk_clicked();
    void timerEvent(QTimerEvent* event);

private:
    Ui::SpriteEditor *ui;
    GMSprite* sprite;
    QImage temp_image;
    QIcon temp_icon;
    int timer_id= 0;
};

#endif // SPRITEEDITOR_H
