#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_closeSubWindow();

private slots:
    void on_btnAddSprite_released();

    void on_btnAddObject_released();

    void on_btnAddRoom_released();

    void on_actionResources_triggered();

    void on_trwResources_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_actionCreate_new_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
