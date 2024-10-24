#include <iostream>
#include <cstring>

#include "../../resman.hpp"
#include "../../gameman.hpp"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "roomorderer.h"

#include <QMessageBox>
#include <QDialog>

QTreeWidgetItem* folder_sprites;
QTreeWidgetItem* folder_objects;
QTreeWidgetItem* folder_rooms;
QTreeWidgetItem* folder_constants;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Hide window controls on the top dock
    ui->dckToolbox->setTitleBarWidget(new QWidget(ui->dckToolbox));

    //Set window title
    this->setWindowTitle(gamename + " - FreeGM");

    mainwindow = this;

    this->showMaximized();

    //Populate resource tree
    //auto tree_model = new QStandardItemModel();
    //QTreeView* restree = new QTreeView(this);
    //restree->setModel(tree_model);
    //restree->model()->;
    //ui->trwResources= restree;

    folder_sprites = ui->trwResources->topLevelItem(FOLDER_SPRITES);
    folder_objects = ui->trwResources->topLevelItem(FOLDER_OBJECTS);
    folder_rooms = ui->trwResources->topLevelItem(FOLDER_ROOMS);
    folder_constants = ui->trwResources->topLevelItem(FOLDER_CONSTANTS);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnAddSprite_released()
{
    QTreeWidgetItem* newitem = resources_newitem("sprite",folder_sprites);
    if (!newitem) return;
    window_open(newitem, ui->mdiDesktop);
}


void MainWindow::on_btnAddObject_released()
{
    QTreeWidgetItem* newitem = resources_newitem("object",folder_objects);
    window_open(newitem, ui->mdiDesktop);
}


void MainWindow::on_btnAddRoom_released()
{
    QTreeWidgetItem* newitem = resources_newitem("room",folder_rooms);
    window_open(newitem, ui->mdiDesktop);
}


void MainWindow::on_actionResources_triggered()
{
    //QDockWidget* newdock = new MainWindow()::ui::dckResources;
    //this->addDockWidget(Qt::LeftDockWidgetArea, newdock);
}


void MainWindow::on_trwResources_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    window_open(item, ui->mdiDesktop);
}

void MainWindow::on_actionCreate_new_triggered()
{
    if (ui->trwResources->selectedItems().count() <= 0)
    {
        //No folder/item selected
        QMessageBox::information(this, "Error creating a new item", "Please select a folder in the resource tree and try again.");
        return;
    }
    QTreeWidgetItem* item = ui->trwResources->selectedItems()[0];

    if (item == folder_sprites || item->parent() == folder_sprites)
        resources_newitem("sprite", folder_sprites);
    if (item == folder_objects || item->parent() == folder_objects)
        resources_newitem("object", folder_objects);
    if (item == folder_rooms || item->parent() == folder_rooms)
        resources_newitem("room", folder_rooms);
    if (item == folder_constants || item->parent() == folder_constants)
        resources_newitem("const", folder_constants);
}

void MainWindow::on_closeSubWindow()
{
    //std::cout << "Closing a subwindow" << std::endl;
    ui->mdiDesktop->closeActiveSubWindow();
}

void MainWindow::on_bntRun_clicked()
{
    //Compile & run game
    game_compile();
}


void MainWindow::on_btnSave_clicked()
{
    //Save game data
    game_save();
}

void MainWindow::on_btnLoad_clicked()
{
    //Load game data
    game_load();
}

void MainWindow::on_btnNew_clicked()
{
    //New game (reset data)
    folder_sprites->takeChildren();
    folder_objects->takeChildren();
    folder_rooms->takeChildren();
    folder_constants->takeChildren();
    resources.clear();
    gamepath= "";
    gamename= "<new game>";
    newgame= true;
    this->setWindowTitle(gamename+"- FreeGM");
}

void MainWindow::on_actionDisable_theme_triggered()
{
    if (this->styleSheet().startsWith("/*"))
        this->setStyleSheet((this->styleSheet()).remove(0,2));
    else
        this->setStyleSheet((this->styleSheet()).insert(0,"/*"));
}

void MainWindow::on_actionDelete_triggered()
{
    //Delete a resource
    if (ui->trwResources->selectedItems().count() <= 0)
    {
        //No folder/item selected
        QMessageBox::information(this, "Error deleting an item", "Please select an item to delete.");
        return;
    }
    QTreeWidgetItem* item = ui->trwResources->selectedItems()[0];
    if (item->parent()==folder_sprites->parent())
    {
        //If the item selected is a folder
        QMessageBox::information(this, "Error deleting an item", "Please select an item to delete.\nCannot delete a resource folder.");
        return;
    }

    if (resources_delitem(item) < 0)
    {
        //something went wrong
        QMessageBox::information(this, "Error deleting an item", "Something went wrong!");
        return;
    }
}

void MainWindow::on_btnRoomOrder_clicked()
{
    RoomOrderer* ro= new RoomOrderer(folder_rooms, this);
    ro->setModal(true);
    ro->show();
}

void MainWindow::on_actionSave_project_legacy_triggered()
{
    game_load_legacy();
}

void MainWindow::on_actionSave_projecacy_t_leg_triggered()
{
    game_save_legacy();
}

void MainWindow::on_actionLoad_triggered()
{
    game_load();
}

void MainWindow::on_actionSave_project_triggered()
{
    game_save();
}

void MainWindow::on_actionSave_project_as_triggered()
{
    QFileDialog idial= QFileDialog();
    idial.setFileMode(QFileDialog::Directory);
    QString newpath= idial.getExistingDirectory(mainwindow, "Choose a folder to save your project in");
    if (newpath == "") return;
    QInputDialog sdial= QInputDialog();
    QString newname= sdial.getText(mainwindow, "Create a new project", "Choose a name for your project");
    if (newname == "") return;

    gamename= newname;
    gamepath= newpath+"/"+newname+"/";
    mainwindow->setWindowTitle(gamename+" - FreeGM");

    game_save();
}
