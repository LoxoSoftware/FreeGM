#include <iostream>

#include "resman.hpp"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    this->setWindowTitle(filename + " - FreeGM");

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
    QTreeWidgetItem* newitem = resources_newitem("sprite",folder_sprites,ui->trwResources);
    if (!newitem) return;
    window_open(newitem, ui->mdiDesktop);
}


void MainWindow::on_btnAddObject_released()
{
    QTreeWidgetItem* newitem = resources_newitem("object",folder_objects,ui->trwResources);
    window_open(newitem, ui->mdiDesktop);
}


void MainWindow::on_btnAddRoom_released()
{
    QTreeWidgetItem* newitem = resources_newitem("room",folder_rooms,ui->trwResources);
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
    QTreeWidgetItem* item = ui->trwResources->selectedItems()[0];

    if (item == folder_sprites || item->parent() == folder_sprites)
        resources_newitem("sprite", folder_sprites, ui->trwResources);
    if (item == folder_objects || item->parent() == folder_objects)
        resources_newitem("object", folder_objects, ui->trwResources);
    if (item == folder_rooms || item->parent() == folder_rooms)
        resources_newitem("room", folder_rooms, ui->trwResources);
    if (item == folder_constants || item->parent() == folder_constants)
        resources_newitem("const", folder_constants, ui->trwResources);
}

void MainWindow::on_closeSubWindow()
{
    //std::cout << "Closing a subwindow" << std::endl;
    ui->mdiDesktop->closeActiveSubWindow();
}
