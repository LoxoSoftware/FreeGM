#ifndef ROOMVIEW_H
#define ROOMVIEW_H

#include "qgraphicsview.h"

#include <QWidget>
#include <QGraphicsWidget>
#include <QGraphicsScene>

class GMRoomSettings;

namespace Ui {
class RoomView;
}

class RoomView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit RoomView(GMRoomSettings* transport, QWidget *parent = nullptr);
    ~RoomView();

public slots:
    void redraw();

private slots:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    Ui::RoomView *ui;
    QGraphicsScene* scene = new QGraphicsScene();
    QBrush bru_fg;
    QBrush bru_bg;
    GMRoomSettings* transport;
};

#endif // ROOMVIEW_H
