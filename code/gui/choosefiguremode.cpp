#include "choosefiguremode.h"
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

ChooseFigureMode::ChooseFigureMode(int w, int h, QWidget *parent)
    : QDialog{parent}
{    
    setFixedSize(w, h);
    setWindowTitle("choose figure mode");

    btn = new QPushButton("确定", this);
    btn->setFixedSize(w>>2, h*0.15);
    btn->move((w >> 1)- (btn->width() >> 1), h * 0.7);

    QLabel* notes = new QLabel(this);
    QPalette label_pe;
    QFont ft;
    ft.setPointSize(20);
    label_pe.setColor(QPalette::WindowText, Qt::red);
    notes->setPalette(label_pe);
    notes->setText("如果需要使用自定义地图，那么就在下方的输入框中输入地图路径后点击确定。\n如果使用默认地图，不需要输入直接点击确定即可。");
    notes->move(0, 0);

    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText("请输入文件路径...");
    lineEdit->setFixedSize(width()*0.8, height()*0.1);
    lineEdit->move((width()>>1)-(lineEdit->width()>>1), (height()>>1)-lineEdit->height());


    connect(btn, &QPushButton::clicked, this, [=](){
        figurePath = lineEdit->text();
        lineEdit->clear();
        emit inputFinished();
    });

    importError = new QErrorMessage(this);
    importError->setFixedSize(w >> 1, h >> 1);
    importError->setWindowTitle("导入提示");
    importError->showMessage("地图导入失败！\n请检查文件路径和格式是否正确！");
    importError->hide();
}

void ChooseFigureMode::paintEvent(QPaintEvent *event)
{
//    QPainter painter(this);
//    QPixmap pix(":/bg/ss_bg.png");
//    painter.drawPixmap(0, 0, width(), height(), pix);


//    chooseFigureMode->show();

}

void ChooseFigureMode::importFailure()
{
    importError->show();
}

