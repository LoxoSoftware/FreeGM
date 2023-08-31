#ifndef GAMEMAN_HPP
#define GAMEMAN_HPP

#endif // GAMEMAN_HPP

#include <QTreeWidgetItem>
#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QFile>
#include <QIODevice>
#include <QProcess>
#include <QThread>

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
                "\tunsigned char pixel_data["+QString::number(spr->image.width())+"*"+
                QString::number(spr->image.height())+"*"+QString::number(spr->image.depth()/8)+"+1];\n"+
                "} "+spr->name+"_s"+" = {\n";
        f_sout.write(data.toLocal8Bit().data());
        data= "\t"+QString::number(spr->image.width())+","+QString::number(spr->image.height())+
                ","+QString::number(spr->image.depth()/8)+",\n\t";
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
        QTreeWidgetItem* item= folder_objects->child(i);
        GMObject* obj= (GMObject*)resource_find(folder_objects->child(i));
        QFile f_oout= QFile(gamepath+"build/objects/"+obj->name+".h");
        cdial->console_write("  writing "+item->text(0)+".h ...");
        f_oout.open(QIODevice::WriteOnly);
        QString data= "__gmklib__object "+obj->name+";\n//Under construction";
        f_oout.write(data.toLocal8Bit().data());

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
        QString data= "__gmklib__room "+room->name+";\n//Under construction";
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
        data+= "\tsprite_add("+spr->name+", "+spr->name+"_s, "+QString::number(spr->frames)+");\n";
    }
    for (int i=0; i<folder_objects->childCount(); i++)
    {
        GMObject* obj= (GMObject*)resource_find(folder_objects->child(i));
        data+= "\tobject_add("+obj->name+", &"+obj->image->name+", "+(obj->visible?QString("true"):QString("false"))+");\n";
    }
    for (int i=0; i<folder_rooms->childCount(); i++)
    {
        GMRoom* room= (GMRoom*)resource_find(folder_rooms->child(i));
        data+= "\troom_add("+room->name+", "+QString::number(room->room_width)+", "+QString::number(room->room_height)
                +", "+QString::number(room->back_color.red())+", "+QString::number(room->back_color.green())
                +", "+QString::number(room->back_color.blue())+", "+(room->fill_back?QString("true"):QString("false"))+");\n";
    }
    data+= "\tcurrent_room= &"+folder_rooms->child(0)->text(0)+";\n";
    f_src.write(data.toLocal8Bit().data());
    data= "";
    GMRoom* first_room= (GMRoom*)resource_find(folder_rooms->child(0));
    for (int i=0; i<first_room->instances.count(); i++)
    {
        data+= "\tinstance_create("+QString::number(first_room->instances[i].x)+", "+QString::number(first_room->instances[i].y)
                +", &"+first_room->instances[i].object->name+");\n";
    }
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
