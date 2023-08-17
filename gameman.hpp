#ifndef GAMEMAN_HPP
#define GAMEMAN_HPP

#endif // GAMEMAN_HPP

#include <QTreeWidgetItem>
#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QFile>
#include <QIODevice>

#include "resman.hpp"
#include "compiledialog.h"

extern QTreeWidgetItem* folder_sprites;
extern QTreeWidgetItem* folder_objects;
extern QTreeWidgetItem* folder_rooms;
extern QTreeWidgetItem* folder_constants;
extern QWidget* mainwindow;

int game_save()
{
    //Returns 0 on success

    if (newgame)
    {
        QFileDialog idial= QFileDialog();
        idial.setFileMode(QFileDialog::Directory);
        QString newpath= idial.getExistingDirectory(mainwindow, "Choose a folder to save your project in");
        if (newpath == "") return 1;
        QInputDialog sdial= QInputDialog();
        QString newname= sdial.getText(mainwindow, "Create a new project", "Choose a name for your project");
        if (newname == "") return 2;

        gamename= newname;
        gamepath= newpath+"/"+newname+"/";
        mainwindow->setWindowTitle(gamename+" - FreeGM");
    }

    QDir().mkpath(gamepath+"sprites/");
    QDir().mkpath(gamepath+"objects/");
    QDir().mkpath(gamepath+"rooms/");
    QDir().mkpath(gamepath+"constants/");

    QFile f_gamedata= QFile(gamepath+gamename+".fgm");
    f_gamedata.open(QIODevice::WriteOnly);
    f_gamedata.write("WARNING! Do not distribute this file alone\n");
    //Save resource tree
    f_gamedata.write((std::to_string(folder_sprites->childCount())).data());
    f_gamedata.write("\n");
    for (int i=0; i<folder_sprites->childCount(); i++)
    {
        f_gamedata.write(folder_sprites->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("\n");
    }
    f_gamedata.write((std::to_string(folder_objects->childCount())).data());
    f_gamedata.write("\n");
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        f_gamedata.write(folder_objects->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("\n");
    }
    f_gamedata.write((std::to_string(folder_rooms->childCount())).data());
    f_gamedata.write("\n");
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        f_gamedata.write(folder_rooms->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("\n");
    }
    f_gamedata.write((std::to_string(folder_constants->childCount())).data());
    f_gamedata.write("\n");
    for (int i=0; i<folder_constants->childCount(); i++)
    {
        f_gamedata.write(folder_constants->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("\n");
    }

    QFile* f= new QFile();

    //Save sprites
    for (int i=0; i<folder_sprites->childCount(); i++)
    {
        GMSprite* spr= ((GMSprite*)treeitem(folder_sprites->child(i)));
        spr->image.save(gamepath+"/sprites/"+spr->name+".png");
        f->setFileName(gamepath+"/sprites/"+spr->name);
        f->open(QIODevice::WriteOnly);
        QString data= QString::number(spr->getFolderIndex())+"\n"+
                       (spr->animated?"true":"false")+"\n"+
                       QString::number(spr->frames);
        f->write(data.toLocal8Bit().data());
        f->close();
    }
    //Save objects
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        GMObject* obj= ((GMObject*)treeitem(folder_objects->child(i)));
        f->setFileName(gamepath+"/objects/"+obj->name);
        f->open(QIODevice::WriteOnly);
        QString data= QString::number(obj->getFolderIndex())+"\n"+
                       (obj->image?obj->image->name:"")+"\n"+
                       (obj->visible?"true":"false")+"\n"+
                       QString::number(obj->events.count())+"\n";
        f->write(data.toLocal8Bit().data());
        //For every event...
        for (int ii=0; ii<obj->events.count(); ii++)
        {
            f->write(obj->events[ii].text().toLocal8Bit().data());
            f->write("\n");
            f->write(obj->event_code[ii].toLocal8Bit().data());
            f->write("\0", 1);
        }
        f->close();
    }
    //Save rooms
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        GMRoom* room= ((GMRoom*)treeitem(folder_rooms->child(i)));
        f->setFileName(gamepath+"/rooms/"+room->name);
        f->open(QIODevice::WriteOnly);
        QString data= QString::number(room->getFolderIndex())+"\n"+
                       QString::number(room->room_width)+"\n"+
                       QString::number(room->room_height)+"\n"+
                       QString::number(room->room_snapX)+"\n"+
                       QString::number(room->room_snapY)+"\n"+
                       QString::number(room->instances.count())+"\n";
        f->write(data.toLocal8Bit().data());
        //For every instance...
        for (int ii=0; ii<room->instances.count(); ii++)
        {
            f->write(room->instances[ii].object->name.toLocal8Bit().data());
            f->write("\n");
            f->write((std::to_string(room->instances[ii].x)).c_str()); f->write("\n");
            f->write((std::to_string(room->instances[ii].y)).c_str()); f->write("\n");
            f->write("\0", 1);
        }
        f->close();
    }
    //Save constants
    for (int i=0; i<folder_constants->childCount(); i++)
    {
        GMConstant* cc= ((GMConstant*)treeitem(folder_constants->child(i)));
        f->setFileName(gamepath+"/constants/"+cc->name);
        f->open(QIODevice::WriteOnly);
        QString data= QString::number(cc->getFolderIndex())+"\n"+
                       cc->value;
        f->write(data.toLocal8Bit().data());
        f->close();
    }

    newgame= false;

    f->close();
    f_gamedata.close();
    return 0;
}

int game_load()
{
    //Returns 0 on success

    QFileDialog idial= QFileDialog();
    QString s_f_gamedata= idial.getOpenFileName(mainwindow, "Open a project file", ".", "FreeGM project file (*.fgm)");

    if (s_f_gamedata == "")
        return 1;

    gamepath= QFileInfo(s_f_gamedata).absolutePath()+"/";
    gamename= QFileInfo(s_f_gamedata).fileName();
    newgame= false;

    //Clear resource tree
    folder_sprites->takeChildren();
    folder_objects->takeChildren();
    folder_rooms->takeChildren();
    folder_constants->takeChildren();
    resources.clear();

    QFile f_gamedata= QFile(s_f_gamedata);
    f_gamedata.open(QIODevice::ReadOnly);
    /*Discard warning text*/ f_gamedata.readLine();
    //Load resource tree
    //Note: file.readLine() does not get rid of the return charater,
    //      it will be removed in resources_loaditem()
    int nres= strtol(f_gamedata.readLine(),nullptr,10);
    for (int i=0; i<nres; i++)
        resources_loaditem(f_gamedata.readLine(),folder_sprites);
    nres= strtol(f_gamedata.readLine(),nullptr,10);
    for (int i=0; i<nres; i++)
        resources_loaditem(f_gamedata.readLine(),folder_objects);
    nres= strtol(f_gamedata.readLine(),nullptr,10);
    for (int i=0; i<nres; i++)
        resources_loaditem(f_gamedata.readLine(),folder_rooms);
    nres= strtol(f_gamedata.readLine(),nullptr,10);
    for (int i=0; i<nres; i++)
        resources_loaditem(f_gamedata.readLine(),folder_constants);

    gamename.chop(gamename.count()-gamename.lastIndexOf(".fgm"));
    mainwindow->setWindowTitle(gamename+" - FreeGM");

    f_gamedata.close();
    return 0;
}

int game_compile()
{
    //Returns 0 on success

    if (newgame)
        if (game_save())
            return 1; //New game is not saved, abort

    QDialog* cdial= new CompileDialog(mainwindow);
    cdial->setParent(mainwindow, Qt::Dialog);
    cdial->setModal(true);
    cdial->open();

    return 0;
}
