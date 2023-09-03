#ifndef VARDEFINER_H
#define VARDEFINER_H

#include <QDialog>

#ifndef RESDEF_HPP
#include "resdef.hpp"
#endif

namespace Ui {
class VarDefiner;
}

class VarDefiner : public QDialog
{
    Q_OBJECT

public:
    explicit VarDefiner(GMObjectVars* transport, QWidget *parent = nullptr);
    ~VarDefiner();

private slots:
    void on_btnOk_clicked();

    void on_btnAdd_clicked();

private:
    Ui::VarDefiner *ui;
    GMObjectVars* transport;
};

#endif // VARDEFINER_H
