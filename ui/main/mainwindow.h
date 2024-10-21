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
    void on_btnSave_clicked();
    void on_bntRun_clicked();
    void on_btnAddSprite_released();
    void on_btnAddObject_released();
    void on_btnAddRoom_released();
    void on_actionResources_triggered();
    void on_trwResources_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_actionCreate_new_triggered();

private slots:
    void on_btnNew_clicked();
    void on_btnLoad_clicked();

    void on_actionDisable_theme_triggered();

    void on_actionDelete_triggered();

    void on_btnRoomOrder_clicked();

    void on_actionSave_projecacy_t_leg_triggered();

    void on_actionLoad_triggered();

    void on_actionSave_project_triggered();

    void on_actionSave_project_as_triggered();

    void on_actionSave_project_legacy_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
