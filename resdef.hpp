#ifndef RESDEF_HPP
#define RESDEF_HPP

#endif // RESDEF_H

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
    int getFolderIndex()
    {
        return folder_index;
    }
    QTreeWidgetItem* getFolder()
    {
        return tree_item->parent();
    }
    QTreeWidgetItem* getTreeItem()
    {
        return tree_item;
    }
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
    GMObject(QTreeWidgetItem* tree_item)
    {
        name= tree_item->text(0);
        this->tree_item= tree_item;
        folder_index= tree_item->parent()->indexOfChild(tree_item);
    }
};

class GMRoom : public GMResource
{
public:
    int room_width = 640;
    int room_height = 480;
    QColor back_color = QColor::fromRgb(0,255,255);
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
    double value= 0.0;
    GMConstant(QTreeWidgetItem* tree_item)
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

class GMRoomSettings
{
public:
    GMRoom* target_room;
    int width;
    int height;
    int snapX;
    int snapY;
    GMObject* selected_object;
    QList<GMInstance> instances;
    GMRoomSettings()
    {

    }
};
