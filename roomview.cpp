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
    if (event->button()==Qt::LeftButton)
        instance_add(event->x(), event->y());
    else
    if (event->button()==Qt::RightButton)
        instance_remove(event->x(), event->y(), false);

    redraw();
}

void RoomView::instance_add(int mouse_x, int mouse_y)
{
    if (transport->selected_object == nullptr)
        return;
    GMInstance* inst;
    transport->instances += GMInstance();
    inst= &transport->instances.last();
    inst->x= mouse_x-(mouse_x%transport->snapX);
    inst->y= mouse_y-(mouse_y%transport->snapY);
    inst->object= transport->selected_object;
}

void RoomView::instance_remove(int mouse_x, int mouse_y, bool all)
{
    for (int i=0; i<transport->instances.count(); i++)
    {
        GMInstance* inst= &transport->instances[i];
        QImage instimg;
        inst->object->image? instimg=inst->object->image->image : instimg=QImage(":/icons/question");
        if (inst->object->image) //Crop the image according to its frame count
                                 //to avoid huge selection areas
            instimg= instimg.copy(0,0,instimg.width()/inst->object->image->frames,instimg.height());

        if ( mouse_x >= inst->x &&
             mouse_x <= inst->x+instimg.width() &&
             mouse_y >= inst->y &&
             mouse_y <= inst->y+instimg.height()
            )
        {
            transport->instances.removeAt(i);
            if (!all) break;
        }
    }
}

void RoomView::redraw()
{
    this->setMaximumSize(transport->width+2, transport->height+2);
    this->setMinimumSize(transport->width+2, transport->height+2);
    this->setSceneRect(0,0,this->width(),this->height());

    bru_fg.setColor(QColor::fromRgb(0,0,0));
    QPen pen= QPen(QColor::fromRgb(0,0,0), 1);
    scene->clear();

    //Draw a grid tile onto a pixmap
    QPixmap grid_pix= QPixmap(":/gfx/fastgrid");
    grid_pix= grid_pix.copy(0,0,transport->snapX,transport->snapY);

    for (int i=0; i<transport->instances.count(); i++)
    {
        //Draw the instances, continue if instance is outside the room
        if (transport->instances[i].x > transport->width ||
            transport->instances[i].y > transport->height)
            continue;
        QPixmap pix;
        if (transport->instances[i].object->image) //Only load the image if the object
                                                   //has a sprite associated to it;
        {
            pix= QPixmap::fromImage(transport->instances[i].object->image->image);

            if (transport->instances[i].object->image->frames>1)
            {
                //Crop the image to the first frame (if it has more than one)
                GMSprite* objspr= transport->instances[i].object->image;
                pix= pix.copy(0,0,objspr->image.width()/objspr->frames,
                               objspr->image.height());
            }
        }
        else
            pix= QPixmap(":/icons/question");

        QGraphicsItem* item= new QGraphicsPixmapItem(pix);
        item->setPos(transport->instances[i].x, transport->instances[i].y);
        scene->addItem(item);
    }

    if (transport->drawGrid && (transport->snapX>2 && transport->snapY>2))
    for (int iy=0; iy<transport->height; iy+=transport->snapY)
    {
        //scene->addLine(0,iy,transport->width,iy,pen);
        for (int ix=0; ix<transport->width; ix+=transport->snapX)
        {
            //scene->addLine(ix,0,ix,transport->height,pen);
            QGraphicsPixmapItem* grid_item= new QGraphicsPixmapItem(grid_pix);
            grid_item->setOffset(ix,iy);
            scene->addItem(grid_item);
        }
    }
}

RoomView::~RoomView()
{
    delete ui;
}
