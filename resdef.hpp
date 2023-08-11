#ifndef RESDEF_HPP
#define RESDEF_HPP

#endif // RESDEF_H

#include <QApplication>
#include <QResource>
#include <QColor>

class GMResource
{
public:
    QString name;
};

class GMSprite : public GMResource
{
public:
    QString image_url;
    bool animated;
    int frames;
    GMSprite()
    {
        image_url= ":/icons/sprite";
        animated= true;
        frames= 1;
    }
};

class GMObject : public GMResource
{
public:
    GMSprite image;
    bool visible;
};

class GMRoom : public GMResource
{
public:
    int room_width = 640;
    int room_height = 480;
    QColor back_color = QColor::fromRgb(0,255,255);
};

class GMConstant : public GMResource
{
public:
    double value= 0.0;
};
