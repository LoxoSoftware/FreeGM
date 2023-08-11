#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#ifndef RESDEF_HPP
#include "resdef.hpp"
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

public slots:
    //void on_loadResource(GMSprite* spr);

private slots:
    void on_btnLoadSprite_clicked();
    void on_btnOk_clicked();

private:
    Ui::SpriteEditor *ui;
    GMSprite* sprite;
};

#endif // SPRITEEDITOR_H
