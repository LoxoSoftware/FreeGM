#ifndef RESDEF_HPP
#define RESDEF_HPP

#endif // RESDEF_H

#include <QApplication>
#include <QResource>
#include <QColor>
#include <QTreeWidgetItem>

class GMResource
{
public:
    QString name;
    void update()
    {
        tree_item->setText(0, name);
    }
    int getFolderIndex()
    {
        return folder_index;
    }
    QTreeWidgetItem* getFolder()
    {
        return tree_item->parent();
    }
protected:
    QTreeWidgetItem* tree_item;
    int folder_index;
};

class GMSprite : public GMResource
{
public:
    QString image_url;
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

        image_url= ":/icons/sprite";
        animated= true;
        frames= 1;
    }
};

class GMObject : public GMResource
{
public:
    GMSprite* image= nullptr;
    bool visible;
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
