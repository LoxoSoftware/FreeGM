#ifndef COMPILEDIALOG_H
#define COMPILEDIALOG_H

#include <QDialog>

namespace Ui {
class CompileDialog;
}

class CompileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompileDialog(QWidget *parent = nullptr);
    ~CompileDialog();

private slots:
    void on_btnStop_clicked();

private:
    Ui::CompileDialog *ui;
};

#endif // COMPILEDIALOG_H
