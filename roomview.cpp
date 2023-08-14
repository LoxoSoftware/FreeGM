#include "roomview.h"
#include "ui_roomview.h"

#include "roomeditor.h"

#include <QGraphicsItem>
#include <QGraphicsWidget>

RoomView::RoomView(GMRoomSettings* transport, QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::RoomView)
{
    ui->setupUi(this);
    this->transport= transport;

    this->setScene(scene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    redraw();
}

void RoomView::mouseMoveEvent(QMouseEvent* event)
{

}

void RoomView::mousePressEvent(QMouseEvent* event)
{
    //this->close();
    //QPixmap pix= QPixmap(":/icons/sprite");
    //int mouse_x = event->x();
    //int mouse_y = event->y();
    //QGraphicsItem* item= new QGraphicsPixmapItem(pix);
    //item->setPos(event->localPos());
    //scene->addItem(item);

    //Place instances

    if (transport->selected_object == nullptr)
        return;

    GMInstance* inst;
    transport->instances += GMInstance();
    inst= &transport->instances.last();
    inst->x= event->x()-(event->x()%transport->snapX);
    inst->y= event->y()-(event->y()%transport->snapY);
    inst->object= transport->selected_object;

    redraw();
}

void RoomView::redraw()
{
    this->setMaximumSize(transport->width+2, transport->height+2);
    this->setMinimumSize(transport->width+2, transport->height+2);
    this->setSceneRect(0,0,this->width(),this->height());

    bru_fg.setColor(QColor::fromRgb(0,0,0));
    QPen pen= QPen(QColor::fromRgb(0,0,0), 1);
    scene->clear();

    for (int i=0; i<transport->instances.count(); i++)
    {

        QPixmap pix;
        if (transport->instances[i].object->image) //Only load the image if the object
                                                   //has a sprite associated to it;
            pix= QPixmap::fromImage(transport->instances[i].object->image->image);
        else
            pix= QPixmap(":/icons/question");

        QGraphicsItem* item= new QGraphicsPixmapItem(pix);
        item->setPos(transport->instances[i].x, transport->instances[i].y);
        scene->addItem(item);
    }

    for (int iy=0; iy<transport->height; iy+=transport->snapY)
    {
        scene->addLine(0,iy,transport->width,iy,pen);
        for (int ix=0; ix<transport->width; ix+=transport->snapX)
        {
            scene->addLine(ix,0,ix,transport->height,pen);
        }
    }
}

RoomView::~RoomView()
{
    delete ui;
}
