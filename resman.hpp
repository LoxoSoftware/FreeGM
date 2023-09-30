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

#include "lib/tinyxml2.h"
using namespace tinyxml2;

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

GMResource* resource_find(QTreeWidgetItem* item)
{
    return resource_find(item->text(0));
}

int resource_index(QString name)
{
    for (int i=0; i<resources.count(); i++)
    {
        if (resources[i]->name == name)
            return i;
    }

    return -1;
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
    newitem->setText(0, name);
    folder->addChild(newitem);

    //Create an object based on that entry and load the data
    GMResource* newres= nullptr;
    if (folder == folder_sprites)
    {
        newres = new GMSprite(newitem);
        XMLDocument f; f.LoadFile(QString(gamepath+"sprites/"+name).toLocal8Bit().data());
        XMLElement* froot= f.FirstChildElement("properties");
        ((GMSprite*)newres)->name= name;
        ((GMSprite*)newres)->image.load(gamepath+"sprites/"+name+".png");
        ((GMSprite*)newres)->icon= QIcon(QPixmap::fromImage(((GMSprite*)newres)->image));
        froot->FirstChildElement("animated")->QueryBoolText(&(((GMSprite*)newres)->animated));
        froot->FirstChildElement("frames")->QueryIntText(&(((GMSprite*)newres)->frames));
    }
    else
    if (folder == folder_objects)
    {
        newres = new GMObject(newitem);
        XMLDocument f; f.LoadFile(QString(gamepath+"objects/"+name).toLocal8Bit().data());
        XMLElement* froot= f.FirstChildElement("properties");
        ((GMObject*)newres)->name= name;
        ((GMObject*)newres)->image= ((GMSprite*)resource_find(froot->FirstChildElement("image")->GetText()));
        froot->FirstChildElement("visible")->QueryBoolText(&(((GMObject*)newres)->visible));
        froot= f.FirstChildElement("events");
        for (XMLElement* tnode= froot->FirstChildElement("event"); tnode; tnode= tnode->NextSiblingElement("event"))
        {
            const char* mstr; tnode->QueryStringAttribute("trigger", &mstr);
            QIcon tico= QIcon(QFile::exists(":/icons/event_"+QString(mstr))?":/icons/event_"+QString(mstr):":icons/question");
            ((GMObject*)newres)->events+=QListWidgetItem(tico,mstr);
            ((GMObject*)newres)->event_code+=tnode->GetText();
        }
    }
    else
    if (folder == folder_rooms)
    {
        newres = new GMRoom(newitem);
        XMLDocument f; f.LoadFile(QString(gamepath+"rooms/"+name).toLocal8Bit().data());
        XMLElement* froot= f.FirstChildElement("properties");
        ((GMRoom*)newres)->name= name;
        froot->FirstChildElement("width")->QueryIntText(&(((GMRoom*)newres)->room_width));
        froot->FirstChildElement("height")->QueryIntText(&(((GMRoom*)newres)->room_height));
        froot->FirstChildElement("snap_width")->QueryIntText(&(((GMRoom*)newres)->room_snapX));
        froot->FirstChildElement("snap_height")->QueryIntText(&(((GMRoom*)newres)->room_snapY));
        ((GMRoom*)newres)->back_color= QColor(froot->FirstChildElement("back_color")->GetText());
        froot->FirstChildElement("fill_back")->QueryBoolText(&(((GMRoom*)newres)->fill_back));
        froot= f.FirstChildElement("instances");
        for (XMLElement* tnode= froot->FirstChildElement("instance"); tnode; tnode= tnode->NextSiblingElement("instance"))
        {
            GMInstance inst= GMInstance();
            const char* mstr= tnode->FirstChildElement("object")->GetText();
            inst.object= ((GMObject*)resource_find(QString(mstr)));
            tnode->FirstChildElement("x")->QueryIntText(&inst.x);
            tnode->FirstChildElement("y")->QueryIntText(&inst.y);
            ((GMRoom*)newres)->instances+=inst;
        }
    }
    else
    if (folder == folder_constants)
    {
        newres = new GMConstant(newitem);
        XMLDocument f; f.LoadFile(QString(gamepath+"rooms/"+name).toLocal8Bit().data());
        XMLElement* froot= f.FirstChildElement("properties");
        ((GMConstant*)newres)->name= name;
        ((GMConstant*)newres)->value= QString(froot->FirstChildElement("value")->GetText());
    }
    else
    return nullptr;
    resources += newres;

    //QTreeWidgetItem* folder = ui->trwResources->findItems(QString::fromStdString("Rooms"),Qt::MatchFlags())[0];

    folder->treeWidget()->expandItem(folder);

    return newitem;
}

QTreeWidgetItem* resources_loaditem_legacy(QString name, QTreeWidgetItem* folder)
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
            QIcon tico= QIcon(QFile::exists(":/icons/event_"+mstr)?":/icons/event_"+mstr:":icons/question");
            ((GMObject*)newres)->events+=QListWidgetItem(tico,mstr);
            char ch= ' '; //Set to anything other than 0 or -1
            while (ch != '\0' && ch != -1)
            {
                f.getChar(&ch);
                if (ch != '\0') tstr+=ch;
            }
            ((GMObject*)newres)->event_code+=tstr;
            ///*Skip null char at the end*/f.getChar(&ch);
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
        ((GMConstant*)newres)->value= f.readLine();
        f.close();
    }
    else
    return nullptr;
    resources += newres;

    //QTreeWidgetItem* folder = ui->trwResources->findItems(QString::fromStdString("Rooms"),Qt::MatchFlags())[0];

    folder->treeWidget()->expandItem(folder);

    return newitem;
}

int resources_delitem(QTreeWidgetItem* item)
{
    //Returns the index of the item removed on success,
    //returns -N on error.
    int ind= resource_index(item->text(0));
    if (ind < 0) return -1; //Item not found error
    resources.removeAt(ind);
    item->parent()->removeChild(item);
    return ind;
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
        widget = new SpriteEditor((GMSprite*)resource_find(item)); else
    if (item->parent() == folder_objects)
        widget = new ObjectEditor((GMObject*)resource_find(item)); else
    if (item->parent() == folder_rooms)
        widget = new RoomEditor((GMRoom*)resource_find(item)); else
    if (item->parent() == folder_constants)
        widget = new ConstantEditor((GMConstant*)resource_find(item)); else
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

