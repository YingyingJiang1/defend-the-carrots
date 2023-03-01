#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QString _normal,QString _pressed, QMainWindow* w,
                      int width, int height,int x, int y, QString buttonName="",QPushButton *parent = nullptr);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private:
    QLabel* label;
    QString normal;
    QString pressed;
signals:



};

#endif // MYBUTTON_H
