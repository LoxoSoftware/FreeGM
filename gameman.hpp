#ifndef GAMEMAN_HPP
#define GAMEMAN_HPP

#include <QTreeWidgetItem>
#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QFile>
#include <QIODevice>
#include <QProcess>
#include <QThread>
#include <iostream>

#include "resman.hpp"
#include "ui/main/compiledialog.h"

#include "lib/tinyxml2.h"

using namespace tinyxml2;

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
    f_gamedata.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    f_gamedata.write("WARNING! Do not distribute this file alone\n");
    //Save resource tree
    f_gamedata.write("<resources>\n");
    f_gamedata.write("<sprites count=\"");
    f_gamedata.write((std::to_string(folder_sprites->childCount())).data());
    f_gamedata.write("\">\n");
    for (int i=0; i<folder_sprites->childCount(); i++)
    {
        f_gamedata.write("<res>");
        f_gamedata.write(folder_sprites->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("</res>\n");
    }
    f_gamedata.write("</sprites>\n");
    f_gamedata.write("<objects count=\"");
    f_gamedata.write((std::to_string(folder_objects->childCount())).data());
    f_gamedata.write("\">\n");
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        f_gamedata.write("<res>");
        f_gamedata.write(folder_objects->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("</res>\n");
    }
    f_gamedata.write("</objects>\n");
    f_gamedata.write("<rooms count=\"");
    f_gamedata.write((std::to_string(folder_rooms->childCount())).data());
    f_gamedata.write("\">\n");
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        f_gamedata.write("<res>");
        f_gamedata.write(folder_rooms->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("</res>\n");
    }
    f_gamedata.write("</rooms>\n");
    f_gamedata.write("<constants count=\"");
    f_gamedata.write((std::to_string(folder_constants->childCount())).data());
    f_gamedata.write("\">\n");
    for (int i=0; i<folder_constants->childCount(); i++)
    {
        f_gamedata.write("<res>");
        f_gamedata.write(folder_constants->child(i)->text(0).toLocal8Bit().data());
        f_gamedata.write("</res>\n");
    }
    f_gamedata.write("</constants>\n");
    f_gamedata.write("</resources>");

    QFile* f= new QFile();

    //Save sprites
    for (int i=0; i<folder_sprites->childCount(); i++)
    {
        GMSprite* spr= ((GMSprite*)resource_find(folder_sprites->child(i)));
        spr->image.save(gamepath+"/sprites/"+spr->name+".png");
        f->setFileName(gamepath+"/sprites/"+spr->name);
        f->open(QIODevice::WriteOnly);
        f->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<properties>\n");
        QString data= "<index>"+QString::number(spr->getFolderIndex())+"</index>\n"+
                      "<animated>"+(spr->animated?"true":"false")+"</animated>\n"+
                      "<frames>"+QString::number(spr->frames)+"</frames>\n";
        f->write(data.toLocal8Bit().data());
        f->write("</properties>");
        f->close();
    }
    //Save objects
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        GMObject* obj= ((GMObject*)resource_find(folder_objects->child(i)));
        f->setFileName(gamepath+"/objects/"+obj->name);
        f->open(QIODevice::WriteOnly);
        f->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<properties>\n");
        QString data= "<index>"+QString::number(obj->getFolderIndex())+"</index>\n"+
                      "<image>"+(obj->image?obj->image->name:"")+"</image>\n"+
                      "<visible>"+(obj->visible?"true":"false")+"</visible>\n";
        f->write(data.toLocal8Bit().data());
        f->write("</properties>\n<variables count=\"");
        f->write(QString::number(obj->variables.count()).toLocal8Bit().data());
        f->write("\">\n");
        for (int ii=0; ii<obj->variables.count(); ii++)
        {
            data= "<variable type=\""+obj->variables_type[ii]+"\" name=\""+
                    obj->variables[ii]+"\">"+obj->variables_val[ii]+"</variable>\n";
            f->write(data.toLocal8Bit().data());
        }
        f->write("</variables>\n<events count=\"");
        f->write(QString::number(obj->events.count()).toLocal8Bit().data());
        f->write("\">\n");
        //For every event...
        for (int ii=0; ii<obj->events.count(); ii++)
        {
            f->write("<event trigger=\"");
            f->write(obj->events[ii].text().toLocal8Bit().data());
            f->write("\">");
            data= "";
            for (int c=0; c<obj->event_code[ii].count(); c++)
            {
                switch (obj->event_code[ii][c].unicode())
                {
                case '<':
                    data+="&lt;";
                    break;
                case '>':
                    data+="&gt;";
                    break;
                case '&':
                    data+="&amp;";
                    break;
                default:
                    data+=obj->event_code[ii][c];
                    break;
                }
            }
            f->write(data.toLocal8Bit().data());
            f->write("</event>\n");
        }
        f->write("</events>\n");
        f->close();
    }
    //Save rooms
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        GMRoom* room= ((GMRoom*)resource_find(folder_rooms->child(i)));
        f->setFileName(gamepath+"/rooms/"+room->name);
        f->open(QIODevice::WriteOnly);
        f->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<properties>\n");
        QString data= "<index>"+QString::number(room->getFolderIndex())+"</index>\n"+
                      "<width>"+QString::number(room->room_width)+"</width>\n"+
                      "<height>"+QString::number(room->room_height)+"</height>\n"+
                      "<snap_width>"+QString::number(room->room_snapX)+"</snap_width>\n"+
                      "<snap_height>"+QString::number(room->room_snapY)+"</snap_height>\n"+
                      "<back_color>"+room->back_color.name()+"</back_color>\n"+
                      "<fill_back>"+(room->fill_back?"true":"false")+"</fill_back>\n";
        f->write(data.toLocal8Bit().data());
        f->write("</properties>\n<instances count=\"");
        f->write(QString::number(room->instances.count()).toLocal8Bit().data());
        f->write("\">\n");
        //For every instance...
        for (int ii=0; ii<room->instances.count(); ii++)
        {
            f->write("<instance>\n<object>");
            f->write(room->instances[ii].object->name.toLocal8Bit().data());
            f->write("</object>\n<x>");
            f->write((std::to_string(room->instances[ii].x)).c_str());
            f->write("</x>\n<y>");
            f->write((std::to_string(room->instances[ii].y)).c_str());;
            f->write("</y>\n</instance>\n");
        }
        f->write("</instances>\n");
        f->close();
    }
    //Save constants
    for (int i=0; i<folder_constants->childCount(); i++)
    {
        GMConstant* cc= ((GMConstant*)resource_find(folder_constants->child(i)));
        f->setFileName(gamepath+"/constants/"+cc->name);
        f->open(QIODevice::WriteOnly);
        f->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<properties>\n");
        QString data= "<index>"+QString::number(cc->getFolderIndex())+"</index>\n"+
                      "<value>"+cc->value+"</value>\n";
        f->write(data.toLocal8Bit().data());
        f->write("</properties>");
        f->close();
    }

    newgame= false;

    f->close();
    f_gamedata.close();
    return 0;
}

int game_save_legacy()
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
        GMSprite* spr= ((GMSprite*)resource_find(folder_sprites->child(i)));
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
        GMObject* obj= ((GMObject*)resource_find(folder_objects->child(i)));
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
        GMRoom* room= ((GMRoom*)resource_find(folder_rooms->child(i)));
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
        GMConstant* cc= ((GMConstant*)resource_find(folder_constants->child(i)));
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

    XMLDocument f_gamedata;
    XMLError eResult = f_gamedata.LoadFile(s_f_gamedata.toLocal8Bit().data());

    XMLElement* pElement = f_gamedata.FirstChildElement("resources")->FirstChildElement("sprites");
    for (XMLElement* pEChild = pElement->FirstChildElement("res"); pEChild; pEChild= pEChild->NextSiblingElement("res"))
        resources_loaditem(pEChild->GetText(), folder_sprites);
    pElement = f_gamedata.FirstChildElement("resources")->FirstChildElement("objects");
    for (XMLElement* pEChild = pElement->FirstChildElement("res"); pEChild; pEChild= pEChild->NextSiblingElement("res"))
        resources_loaditem(pEChild->GetText(), folder_objects);
    pElement = f_gamedata.FirstChildElement("resources")->FirstChildElement("rooms");
    for (XMLElement* pEChild = pElement->FirstChildElement("res"); pEChild; pEChild= pEChild->NextSiblingElement("res"))
        resources_loaditem(pEChild->GetText(), folder_rooms);
    pElement = f_gamedata.FirstChildElement("resources")->FirstChildElement("constants");
    for (XMLElement* pEChild = pElement->FirstChildElement("res"); pEChild; pEChild= pEChild->NextSiblingElement("res"))
        resources_loaditem(pEChild->GetText(), folder_constants);

    gamename.chop(gamename.count()-gamename.lastIndexOf(".fgm"));
    mainwindow->setWindowTitle(gamename+" - FreeGM");

    return 0;
}

int game_load_legacy()
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
        resources_loaditem_legacy(f_gamedata.readLine(),folder_sprites);
    nres= strtol(f_gamedata.readLine(),nullptr,10);
    for (int i=0; i<nres; i++)
        resources_loaditem_legacy(f_gamedata.readLine(),folder_objects);
    nres= strtol(f_gamedata.readLine(),nullptr,10);
    for (int i=0; i<nres; i++)
        resources_loaditem_legacy(f_gamedata.readLine(),folder_rooms);
    nres= strtol(f_gamedata.readLine(),nullptr,10);
    for (int i=0; i<nres; i++)
        resources_loaditem_legacy(f_gamedata.readLine(),folder_constants);

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

    CompileDialog* cdial= new CompileDialog(mainwindow);
    cdial->setParent(mainwindow, Qt::Dialog);
    cdial->setModal(true);
    cdial->open();

    cdial->set_progress(0);
    cdial->console_clear();

    if (folder_rooms->childCount() <= 0)
    {
        cdial->console_write("ERROR! A game needs at least one room to start");
        return 2;
    }

    cdial->console_write("Preparing build directory...");
    QDir().mkpath(gamepath+"build/");
    QDir().mkpath(gamepath+"build/gfx/");
    QDir().mkpath(gamepath+"build/objects/");
    QDir().mkpath(gamepath+"build/rooms/");

    //Add the makefile
    cdial->console_write("Adding the makefile...");
    QFile f_make= QFile(gamepath+"build/Makefile");
    f_make.open(QIODevice::WriteOnly);
    f_make.write("BIN_NAME := ");
    f_make.write(gamename.toLocal8Bit());
    f_make.write("\nall:\n");
    f_make.write("\tgcc main.cpp -o $(BIN_NAME) -s -L -lSDL2_image -lSDL2_ttf -lSDL2 -lm ");
    f_make.close();

    //Add game library file
    cdial->console_write("Adding game library...");
    QFile f_gmkl= QFile(gamepath+"build/gmklib-sdl.h");
    f_gmkl.open(QIODevice::WriteOnly);
    f_gmkl.write(QResource(":/res/gmklib-sdl").uncompressedData());
    f_gmkl.close();

    cdial->console_write("Converting images...");
    //Export images to csource (GIMP-like format)
    for (int i=0; i<folder_sprites->childCount(); i++)
    {
        QTreeWidgetItem* item= folder_sprites->child(i);
        GMSprite* spr= (GMSprite*)resource_find(folder_sprites->child(i));
        QFile f_sout= QFile(gamepath+"build/gfx/"+spr->name+".h");
        cdial->console_write("  writing "+item->text(0)+".h ...");
        if (!spr)
        {
            cdial->console_write("Error writing sprite data!");
            return 1;
        }
        spr->image= spr->image.convertToFormat(QImage::Format_RGBA8888);
        f_sout.open(QIODevice::WriteOnly);
        QString data= "__gmklib__sprite "+item->text(0)+";\n"+
                "static const struct {\n\tunsigned int  width;\n"+
                "\tunsigned int  height;\n\tunsigned int  bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */\n"
                "\tunsigned int  frames;\n"
                "\tunsigned char pixel_data["+QString::number(spr->image.width())+"*"+
                QString::number(spr->image.height())+"*"+QString::number(spr->image.depth()/8)+"+1];\n"+
                "} "+spr->name+"_s"+" = {\n";
        f_sout.write(data.toLocal8Bit().data());
        data= "\t"+QString::number(spr->image.width())+","+QString::number(spr->image.height())+
                ","+QString::number(spr->image.depth()/8)+","+QString::number(spr->frames)+",\n\t";
        f_sout.write(data.toLocal8Bit().data());
        data= "";
        const unsigned char* img_bytes= spr->image.bits();
        for (long i=0; i<spr->image.sizeInBytes(); i++)
        {
            data+= "0x"+QString::number(img_bytes[i],16)+",";
            if (i%16 == 15) data+= "\n\t";
        }
        f_sout.write(data.toLocal8Bit().data());
        f_sout.write("\n};");

        f_sout.close();
    }
    cdial->console_write("Adding objects...");
    //Export objects
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        QList<QString> events= QList<QString>();
        events+="onCreate";
        events+="onBeginFrame";
        events+="onEndFrame";
        events+="onDraw";
        events+="onDestroy";
        QTreeWidgetItem* item= folder_objects->child(i);
        GMObject* obj= (GMObject*)resource_find(folder_objects->child(i));
        QFile f_oout= QFile(gamepath+"build/objects/"+obj->name+".h");
        cdial->console_write("  writing "+item->text(0)+".h ...");
        f_oout.open(QIODevice::WriteOnly);
        QString vardef_data= "";
        QString varget_data= "";
        QString varend_data= "";
        QString data= "__gmklib__object "+obj->name+";";
        bool new_onCreate=true; //onCreate event is not present, need to make a new one
        for (int ii=0; ii<obj->events.count(); ii++)
        {
            data+="\nvoid __"+obj->name+"__"+obj->events[ii].text()+"(__gmklib__instance* self);";
            if (obj->events[ii].text() == events[0]) new_onCreate= false;
        }
        if (new_onCreate)
            data+="\nvoid __"+obj->name+"__"+events[0]+"(__gmklib__instance* self);";
        data+="\nstatic struct {\n\tstruct __gmklib__sprite_s* sprite;\n\tbool visible;\n";
        for (int ii=0; ii<events.count(); ii++)
            data+="\tvoid (*"+events[ii]+")(__gmklib__instance*);\n";
        data+="} "+obj->name+"_s = {\n\t"+(obj->image?"&"+obj->image->name:"NULL")+","+
                (obj->visible?QString("true"):QString("false"))+",\n";
        f_oout.write(data.toLocal8Bit().data());
        data= "";
        for (int ii=0; ii<obj->variables.count(); ii++)
            vardef_data+="\t"+obj->variables_type[ii]+"* "+obj->variables[ii]+"_ptr= ("
                    +obj->variables_type[ii]+"*)instance_add_variable(self, \""
                    +obj->variables[ii]+"\", "+obj->variables_type[ii]+");\n";
        if (!new_onCreate)
        for (int ii=0; ii<obj->variables.count(); ii++)
            vardef_data+="\t"+obj->variables_type[ii]+" "+obj->variables[ii]+"= "
                    +"*"+obj->variables[ii]+"_ptr;\n";
        for (int ii=0; ii<obj->variables.count(); ii++)
            varget_data+="\t"+obj->variables_type[ii]+"* "+obj->variables[ii]+"_ptr= ("
                    +obj->variables_type[ii]+"*)instance_get_variable(self, \""
                    +obj->variables[ii]+"\");\n\t"+obj->variables_type[ii]+" "+obj->variables[ii]
                    +"= *"+obj->variables[ii]+"_ptr;\n";
        for (int ii=0; ii<obj->variables.count(); ii++)
            varend_data+="\t*"+obj->variables[ii]+"_ptr="+obj->variables[ii]+";\n";
        for (int ii=0; ii<events.count(); ii++)
        {
            //Find if the object has the trigger defined,
            //if not, the fn pointer will point to NULL
            bool found= false;
            for (int iii=0; iii<obj->events.count(); iii++)
            {
                if (obj->events[iii].text() == events[ii])
                {
                    data+= "\t&__"+obj->name+"__"+events[ii]+",\n";
                    found=true;
                }
            }
            if (new_onCreate && ii==0)
                data+= "\t&__"+obj->name+"__"+events[0]+",\n";
            else
            if (!found) data+= "\tNULL,\n";
        }
        data+= "};\n";
        f_oout.write(data.toLocal8Bit().data());
        if (new_onCreate)
        {
            data="\nvoid __"+obj->name+"__"+events[0]+"(__gmklib__instance* self)\n{\n\t"+
                    "//NOTE: Empty event\n"+vardef_data+"\n}\n";
            f_oout.write(data.toLocal8Bit().data());
        }
        for (int ii=0; ii<obj->events.count(); ii++)
        {
            data="\nvoid __"+obj->name+"__"+obj->events[ii].text()+"(__gmklib__instance* self)\n{\n"+
                    (obj->events[ii].text()==events[0]?QString(vardef_data+""):QString(varget_data+"\n"))+obj->event_code[ii]+"\n\n"
                    +varend_data+"}\n";
            f_oout.write(data.toLocal8Bit().data());
        }

        f_oout.close();
    }
    cdial->console_write("Adding rooms...");
    //Export rooms
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        QTreeWidgetItem* item= folder_rooms->child(i);
        GMRoom* room= (GMRoom*)resource_find(folder_rooms->child(i));
        QFile f_rout= QFile(gamepath+"build/rooms/"+room->name+".h");
        cdial->console_write("  writing "+item->text(0)+".h ...");
        f_rout.open(QIODevice::WriteOnly);
        QString data= "__gmklib__room "+room->name+";\n"+
                "static struct {\n\tunsigned int width;\n\tunsigned int height;\n"+
                "\tSDL_Color back_color; //NOTE: RGBA color\n\tbool fill_back;\n"+
                "\tstruct __gmklib__instance_s instances["+QString::number(room->instances.count())+"];\n"+
                "} "+room->name+"_s = {"+
                "\n\t"+QString::number(room->room_width)+","+QString::number(room->room_height)+","+
                QString::number(room->back_color.red())+","+QString::number(room->back_color.green())+","+
                QString::number(room->back_color.blue())+",255,"+(room->fill_back?QString("true"):QString("false"));
        f_rout.write(data.toLocal8Bit().data());
        data= ",\n\t";
        for (int i=0; i<room->instances.count(); i++)
        {
            data+="&"+room->instances[i].object->name+","+QString::number(room->instances[i].x)+
                    ","+QString::number(room->instances[i].y)+",NULL,NULL, ";
            if (i%2==1) data+= "\n\t";
        }
        data+="\n};";
        f_rout.write(data.toLocal8Bit().data());

        f_rout.close();
    }

    //Build main.cpp
    cdial->console_write("Building main.cpp...");
    QFile f_src= QFile(gamepath+"build/main.cpp");
    f_src.open(QIODevice::WriteOnly);
    QString data= "#include \"gmklib-sdl.h\"\n\n";
    for (int i=0; i<folder_sprites->childCount(); i++)
    {
        GMSprite* spr= (GMSprite*)resource_find(folder_sprites->child(i));
        data+="#include \"gfx/"+spr->name+".h\"\n";
    }
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        GMObject* obj= (GMObject*)resource_find(folder_objects->child(i));
        data+="#include \"objects/"+obj->name+".h\"\n";
    }
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        GMRoom* room= (GMRoom*)resource_find(folder_rooms->child(i));
        data+="#include \"rooms/"+room->name+".h\"\n";
    }
    data+= "\nint __loop= 1;\n";
    f_src.write(data.toLocal8Bit().data());
    data= "int main(int argc, char* argv[])\n{\n\tdisplay_init(720, 540, \""+gamename+"\", 0);\n";
    for (int i=0; i<folder_sprites->childCount(); i++)
    {
        GMSprite* spr= (GMSprite*)resource_find(folder_sprites->child(i));
        data+= "\tsprite_add("+spr->name+", "+spr->name+"_s);\n";
    }
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        GMObject* obj= (GMObject*)resource_find(folder_objects->child(i));
        data+= "\tobject_add("+obj->name+", "+obj->name+"_s);\n";
    }
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        GMRoom* room= (GMRoom*)resource_find(folder_rooms->child(i));
        data+= "\troom_add("+room->name+", "+room->name+"_s);\n";
    }
    data+= "\troom_goto(&"+folder_rooms->child(0)->text(0)+");\n";
    f_src.write(data.toLocal8Bit().data());
    data = "\n\twhile (__loop)\n\t{\n\t\tdo_update();\n\t\tevent_poll();\n\t\tif (check_exit()) __loop= 0;\n\n";
    data+= "\t\tdraw_current_room();\n";
    data+= "\t\tdisplay_render();\n\t}\n";
    data+= "\n\tdisplay_destroy();\n\treturn 0;\n}";
    f_src.write(data.toLocal8Bit().data());
    f_src.close();

    cdial->console_write("make -C "+gamepath+"build/");
    cdial->set_progress(50);
    QProcess p_gpp= QProcess();
    QStringList args= QStringList();
    args+= "-C"+gamepath+"build/";
    p_gpp.start(QString("make"), args);
    p_gpp.waitForStarted();
    p_gpp.waitForFinished();
    int ret= p_gpp.exitCode();
    if (ret)
    {
        cdial->console_write("ERROR! Command returned "+QString::number(ret)+"!");
        return 1;
    }

    cdial->set_progress(100);
    cdial->console_write("Done! Starting game...");

    QProcess* p_game= new QProcess();
    p_game->start(gamepath+"build/"+gamename, QStringList());
    p_game->waitForStarted();
    p_game->waitForFinished();
    cdial->close();
    return 0;
}

#endif // GAMEMAN_HPP
