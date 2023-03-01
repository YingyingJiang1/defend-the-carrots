#include "mybutton.h"
#include <QPushButton>
#include <QLabel>

MyButton::MyButton(QString _normal,QString _pressed,QMainWindow* w,
                   int width, int height,int x, int y, QString buttonName, QPushButton *parent)
    : QPushButton{parent}
{
    normal = _normal;
    pressed = _pressed;
    this->setParent(w);
    this->setText(buttonName);
    this->resize(width, height);
    //this->setFixedSize(this->width()*1.5, this->height()*2);
    this->move(x, y);


    // Adds a picture and uses it to cover the start button
    label = new QLabel();
    QPixmap pix = QPixmap(normal);
    pix = pix.scaled(width, height,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    label->setParent(w);
    label->setFixedSize(width, height);
    label->setPixmap(pix);
    label->move(x, y);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);
    w->setStyleSheet("QPushButton{border:0px}");
}

void MyButton::mousePressEvent(QMouseEvent* event)
{
    if(pressed.isEmpty())
        return QPushButton::mousePressEvent(event);
    QPixmap pix = QPixmap(pressed);
    pix = pix.scaled(label->width(), label->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(pix);
    return QPushButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent* event)
{
    QPixmap pix = QPixmap(normal);
    pix = pix.scaled(label->width(), label->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    label->setPixmap(pix);
    return QPushButton::mouseReleaseEvent(event);
}
