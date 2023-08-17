#ifndef RESMAN_HPP
#define RESMAN_HPP

#include <QApplication>
#include <QTreeWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QList>

#include <iostream>
#include <string>

#include "resdef.hpp"

QString gamepath = "<null path>";
QString gamename = "<new game>";
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

QWidget* mainwindow;

QList<GMResource*> resources= QList<GMResource*>();

#include "spriteeditor.h"
#include "roomeditor.h"
#include "objecteditor.h"
#include "constanteditor.h"

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

QTreeWidgetItem* resources_newitem(QString default_name, QTreeWidgetItem* folder)
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

    folder->treeWidget()->expandItem(folder);

    return newitem;
}

QTreeWidgetItem* resources_loaditem(QString name, QTreeWidgetItem* folder)
{
    QTreeWidgetItem* newitem = new QTreeWidgetItem();

    //Create an entry in the resource tree
    name.chop(name.count()-name.indexOf('\n')); //Remove newline from the name
    newitem->setText(0, name);
    folder->addChild(newitem);

    //Create an object based on that entry and load the data
    GMResource* newres= nullptr;
    if (folder == folder_sprites)
    {
        newres = new GMSprite(newitem);
        QFile f= QFile(gamepath+"sprites/"+name);
        f.open(QFile::ReadOnly);
        ((GMSprite*)newres)->name= name;
        ((GMSprite*)newres)->image.load(gamepath+"sprites/"+name+".png");
        ((GMSprite*)newres)->icon= QIcon(QPixmap::fromImage(((GMSprite*)newres)->image));
        /*Discard folder index*/ f.readLine();
        ((GMSprite*)newres)->animated= f.readLine()=="true\n"?true:false;
        ((GMSprite*)newres)->frames= strtol(f.readLine(),nullptr,10);
        f.close();
    }
    else
    if (folder == folder_objects)
    {
        newres = new GMObject(newitem);
        QFile f= QFile(gamepath+"objects/"+name);
        f.open(QFile::ReadOnly);
        ((GMObject*)newres)->name= name;
        /*Discard folder index*/ f.readLine();
        QString mstr= f.readLine(); mstr.chop(mstr.count()-mstr.indexOf('\n'));
        ((GMObject*)newres)->image= ((GMSprite*)resource_find(mstr));
        ((GMObject*)newres)->visible= f.readLine()=="true\n"?true:false;
        int nevents= strtol(f.readLine(),nullptr,10);
        for (int i=0; i<nevents; i++)
        {
            mstr= f.readLine(); mstr.chop(mstr.count()-mstr.indexOf('\n'));
            QString tstr= "";
            ((GMObject*)newres)->events+=QListWidgetItem(QIcon(":/icons/question"),mstr);
            char ch= ' '; //Set to anything other than 0 or -1
            while (ch != '\0' && ch != -1)
            {
                f.getChar(&ch);
                tstr+=ch;
            }
            ((GMObject*)newres)->event_code+=tstr;
            /*Skip null char at the end*/f.getChar(&ch);
        }
        f.close();
    }else
    if (folder == folder_rooms)
    {
        newres = new GMRoom(newitem);
        QFile f= QFile(gamepath+"rooms/"+name);
        f.open(QFile::ReadOnly);
        ((GMRoom*)newres)->name= name;
        /*Discard folder index*/ f.readLine();
        ((GMRoom*)newres)->room_width= strtol(f.readLine(),nullptr,10);
        ((GMRoom*)newres)->room_height= strtol(f.readLine(),nullptr,10);
        ((GMRoom*)newres)->room_snapX= strtol(f.readLine(),nullptr,10);
        ((GMRoom*)newres)->room_snapY= strtol(f.readLine(),nullptr,10);
        int ninst= strtol(f.readLine(),nullptr,10);
        for (int i=0; i<ninst; i++)
        {
            GMInstance inst= GMInstance();
            QString mstr= f.readLine(); mstr.chop(mstr.count()-mstr.indexOf('\n')); //obj name
            inst.object= ((GMObject*)resource_find(mstr));
            inst.x= strtol(f.readLine(),nullptr,10);
            inst.y= strtol(f.readLine(),nullptr,10);
            ((GMRoom*)newres)->instances+=inst;
            /*Skip null char at the end*/f.read(1);
        }
        f.close();
    }
    else
    if (folder == folder_constants)
    {
        newres = new GMConstant(newitem);
        QFile f= QFile(gamepath+"constants/"+name);
        f.open(QFile::ReadOnly);
        ((GMConstant*)newres)->name= name;
        /*Discard folder index*/ f.readLine();
        ((GMConstant*)newres)->value= std::stod(f.readLine().toStdString());
        f.close();
    }
    else
    return nullptr;
    resources += newres;

    //QTreeWidgetItem* folder = ui->trwResources->findItems(QString::fromStdString("Rooms"),Qt::MatchFlags())[0];

    folder->treeWidget()->expandItem(folder);

    return newitem;
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
        widget = new ObjectEditor((GMObject*)treeitem(item)); else
    if (item->parent() == folder_rooms)
        widget = new RoomEditor((GMRoom*)treeitem(item)); else
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

#endif // RESMAN_HPP
