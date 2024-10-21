#ifndef EVENTPICKER_H
#define EVENTPICKER_H

#include <QDialog>
#include <QTreeWidget>
#include <QListWidget>

namespace Ui {
class EventPicker;
}

class EventPicker : public QDialog
{
    Q_OBJECT

public:
    explicit EventPicker(QListWidget* output, QWidget *parent = nullptr);
    ~EventPicker();

public slots:
    void on_trwEvents_itemActivated(QTreeWidgetItem *item, int column);

private slots:
    void on_btnCancel_clicked();

private:
    Ui::EventPicker *ui;
    QListWidget* event_list;
};

#endif // EVENTPICKER_H
