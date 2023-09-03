#ifndef RESDEF_HPP
#define RESDEF_HPP

#include <QApplication>
#include <QResource>
#include <QColor>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

class GMResource
{
public:
    QString name;
    void update()
    {
        tree_item->setText(0, name);
        tree_item->treeWidget()->setCurrentItem(tree_item);
    }
    int getFolderIndex() { return folder_index; }
    QTreeWidgetItem* getTreeItem() { return tree_item; }
protected:
    QTreeWidgetItem* tree_item;
    int folder_index;
};

class GMSprite : public GMResource
{
public:
    //QString image_url;
    QIcon icon;
    QImage image;
    bool animated;
    int frames;
    GMSprite(QTreeWidgetItem* tree_item)
    {
        if (!tree_item)
        {
            throw "Error in initializing GMSprite: tree_item is nullptr.";
            return;
        }
        name= tree_item->text(0);
        this->tree_item= tree_item;
        folder_index= tree_item->parent()->indexOfChild(tree_item);

        //image_url= ":/icons/sprite";
        icon= QIcon(":/icons/sprite");
        image= QImage(":/icons/sprite");
        animated= true;
        frames= 1;
    }
};

class GMObject : public GMResource
{
public:
    GMSprite* image= nullptr;
    bool visible;
    QList<QListWidgetItem> events;
    QList<QString> event_code;
    QList<QString> variables;
    QList<QString> variables_val;
    GMObject(QTreeWidgetItem* tree_item)
    {
        name= tree_item->text(0);
        this->tree_item= tree_item;
        folder_index= tree_item->parent()->indexOfChild(tree_item);
    }
};

class GMInstance
{
public:
    int x;
    int y;
    GMObject* object;
};

class GMRoom : public GMResource
{
public:
    int room_width = 640;
    int room_height = 480;
    int room_snapX = 32;
    int room_snapY = 32;
    bool room_grid = true;
    QList<GMInstance> instances;
    QColor back_color = QColor::fromRgb(0,255,255);
    bool fill_back = true;
    GMRoom(QTreeWidgetItem* tree_item)
    {
        name= tree_item->text(0);
        this->tree_item= tree_item;
        folder_index= tree_item->parent()->indexOfChild(tree_item);
    }
};

class GMConstant : public GMResource
{
public:
    QString value= "";
    GMConstant(QTreeWidgetItem* tree_item)
    {
        name= tree_item->text(0);
        this->tree_item= tree_item;
        folder_index= tree_item->parent()->indexOfChild(tree_item);
    }
};

class GMRoomSettings
{
public:
    GMRoom* target_room;
    int width;
    int height;
    int snapX;
    int snapY;
    bool drawGrid;
    QColor back_color;
    bool fill_back;
    GMObject* selected_object;
    QList<GMInstance> instances;
    GMRoomSettings() {}
};

class GMObjectVars //Custom variables def. transport class
{
public:
    GMObject* target_object;
    QList<QString> variables;
    QList<QString> variables_val;
    GMObjectVars() {}
};

#endif // RESDEF_H

