#ifndef ROOMORDERER_H
#define ROOMORDERER_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class RoomOrderer;
}

class RoomOrderer : public QDialog
{
    Q_OBJECT

public:
    explicit RoomOrderer(QTreeWidgetItem* folder, QWidget *parent = nullptr);
    ~RoomOrderer();

private slots:
    void on_btnCancel_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnOk_clicked();

private:
    Ui::RoomOrderer *ui;
    QTreeWidgetItem* folder;
};

#endif // ROOMORDERER_H
