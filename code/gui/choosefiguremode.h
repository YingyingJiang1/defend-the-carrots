#ifndef CHOOSEFIGUREMODE_H
#define CHOOSEFIGUREMODE_H

#include <QWidget>
#include <QPaintEvent>
#include <QDialog>
#include <QString>
#include <QErrorMessage>

class ChooseFigureMode : public QDialog
{
    Q_OBJECT
    QPushButton * btn;
    QString figurePath;
    QErrorMessage* importError;
public:
    explicit ChooseFigureMode(int w, int h, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    QString& getFigurePath(){return figurePath;}
    void reset(){figurePath.clear();}
    void importFailure();

signals:
    void inputFinished();

};

#endif // CHOOSEFIGUREMODE_H
