#ifndef RESMAN_HPP
#define RESMAN_HPP

#endif // RESMAN_HPP

#include <QApplication>
#include <QTreeWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QList>

#include <iostream>

#include "resdef.hpp"

QString filename = "<new game>";
bool newgame = true;

#define FOLDER_SPRITES      0
#define FOLDER_OBJECTS      1
#define FOLDER_ROOMS        2
#define FOLDER_CONSTANTS    3

extern QTreeWidgetItem* folder_sprites;
extern QTreeWidgetItem* folder_objects;
extern QTreeWidgetItem* folder_rooms;
extern QTreeWidgetItem* folder_constants;
//class GMResource;
//class GMSprite;
//class GMObject;
//class GMRoom;
//class GMConstant;

QList<GMResource*> resources= QList<GMResource*>();

#include "spriteeditor.h"
#include "roomeditor.h"
#include "objecteditor.h"
#include "constanteditor.h"

QTreeWidgetItem* resources_newitem(QString default_name, QTreeWidgetItem* folder, QTreeWidget* tree)
{
    QString newname = default_name+QString::number(folder->childCount());
    QTreeWidgetItem* newitem = new QTreeWidgetItem();

    //Create an entry in the resource tree
    newitem->setText(0, newname);
    folder->addChild(newitem);

    //Create an object based on that entry
    GMResource* newres= nullptr;
    if (folder == folder_sprites)
        newres = new GMSprite(newitem); else
    if (folder == folder_objects)
        newres = new GMObject(newitem); else
    if (folder == folder_rooms)
        newres = new GMRoom(newitem); else
    if (folder == folder_constants)
        newres = new GMConstant(newitem); else
    return nullptr;
    resources += newres;

    //QTreeWidgetItem* folder = ui->trwResources->findItems(QString::fromStdString("Rooms"),Qt::MatchFlags())[0];

    tree->expandItem(folder);

    return newitem;
}

GMResource* resource_find(QString name)
{
    for (int i=0; i<resources.count(); i++)
    {
        if (resources[i]->name == name)
            return resources[i];
    }

    return nullptr;
}

GMResource* treeitem(QTreeWidgetItem* item)
{
    /*for (int i=0; i<tree->children().count(); i++)
    {
        if ((QTreeWidgetItem*)tree->children()[i] == item)
            return &resources[i];
    }*/
    std::cout << "Found resource: " << item->text(0).toStdString() << std::endl;
    return resource_find(item->text(0));
}

/*
QWidget* window_open(int wintype, QMdiArea* mdidesktop, QObject* resptr = nullptr) //Reference by int
{
    QWidget* newwindow;

    if (wintype == FOLDER_SPRITES)
        newwindow = new SpriteEditor(nullptr); else
    if (wintype == FOLDER_OBJECTS)
        newwindow = new ObjectEditor(); else
    if (wintype == FOLDER_ROOMS)
        newwindow = new RoomEditor(); else
    if (wintype == FOLDER_CONSTANTS)
        newwindow = new ConstantEditor(); else
    return nullptr;

    mdidesktop->addSubWindow(newwindow);
    newwindow->show();

    return newwindow;
}*/

QWidget* window_open(QTreeWidgetItem* item, QMdiArea* mdidesktop) //Reference by item
{
    QWidget* widget;

    if (item->parent() == folder_sprites)
        widget = new SpriteEditor((GMSprite*)treeitem(item)); else
    if (item->parent() == folder_objects)
        widget = new ObjectEditor((GMObject*)treeitem(item), folder_sprites); else
    if (item->parent() == folder_rooms)
        widget = new RoomEditor(); else
    if (item->parent() == folder_constants)
        widget = new ConstantEditor(); else
    return nullptr;

    QMdiSubWindow* newwindow =
        mdidesktop->addSubWindow(widget);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    newwindow->setAttribute(Qt::WA_DeleteOnClose);
    int titlebar_h = newwindow->style()->pixelMetric(QStyle::PM_TitleBarHeight);
    newwindow->setMaximumSize(widget->maximumWidth()+6, widget->maximumHeight()+titlebar_h+8);
    newwindow->setMinimumSize(widget->minimumWidth()+6, widget->minimumHeight()+titlebar_h+8);
    newwindow->show();
    //This allows the window to be closed
    QObject::connect(widget,SIGNAL(destroyed(QObject*)),mdidesktop->parent()->parent(),SLOT(on_closeSubWindow()));
    return widget;
}
