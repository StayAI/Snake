#include "widget.h"
#include "ui_widget.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QThread>
#include <QBrush>
#include "snake.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    pHead = new Snake;
}

Widget::~Widget()
{
    delete ui;
}


void Widget::dealSignal()
{
    ui->lcdNumber->display(pHead->getLength());

    qDebug() << pHead->getLength();
    if (pHead->getLength() < 3)
        pHead->Create_Snake(pHead);

    pHead->Snake_Move(pHead);                       //蛇移动
    pHead->Eat_Point(pHead);						//吃到食物了

    pHead->Judge_Wall(pHead);						//判断是否撞墙
    pHead->Judge_itself(pHead);                     //判断是否撞自己

    update();
}


void Widget::paintEvent(QPaintEvent *)
{
    gridW = width() / 16;
    gridH = height() / 12;

    startX = gridW;
    startY = gridH;

    QPainter p(this);
    QPen pen;
    pen.setWidth(2);
    p.setPen(pen);//将画笔交给画家

    for (int i=0; i<11; i++)
    {
        p.drawLine(startX, startY+i*gridH, startX+14*gridW, startY+i*gridH);
    }
    for (int i=0; i<15; i++)
    {
        p.drawLine(startX+i*gridW, startY, startX+i*gridW, startY+10*gridH);
    }

    //创建画刷对象
    QBrush brush;
    brush.setColor( Qt::red );//设置颜色
    brush.setStyle( Qt::Dense4Pattern);//设置样式

    p.setBrush(brush);

    Snake* ptemp = new Snake;
    ptemp = pHead->getpNext();
    while (ptemp != 0)
    {
        p.drawRect(ptemp->getNow().x(),  ptemp->getNow().y(), pHead->getSize(), pHead->getSize());
        ptemp = ptemp->getpNext();
    }
    brush.setColor( Qt::green );//设置颜色
    p.setBrush(brush);
    p.drawRect(pHead->getpTail()->getNow().x(),  pHead->getpTail()->getNow().y(), pHead->getSize(), pHead->getSize());

    brush.setColor( Qt::blue );//设置颜色
    p.setBrush(brush);
    p.drawRect(pHead->getNow().x(),  pHead->getNow().y(), pHead->getSize(), pHead->getSize());

    brush.setColor( Qt::black );//设置颜色
    p.setBrush(brush);
    p.drawRect(pHead->getSrandPoint().x(), pHead->getSrandPoint().y(), pHead->getSize(), pHead->getSize());
}

void Widget::on_buttonStart_clicked()
{
    timerId = this->startTimer(100);//毫秒 每隔一秒触发一次定时器
}

void Widget::on_buttonStop_clicked()
{
    this->killTimer(timerId);
}
void Widget::timerEvent(QTimerEvent *)
{
    dealSignal();
}

int num = 100;

void Widget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_W || e->key() == Qt::Key_Up)
    {
        if(pHead->getDirection()==left_ || pHead->getDirection()==right_)
        pHead->setDirection(up_);
    }
    if (e->key() == Qt::Key_S || e->key() == Qt::Key_Down)
    {
        if(pHead->getDirection()==left_ || pHead->getDirection()==right_)
        pHead->setDirection(down_);
    }
    if (e->key() == Qt::Key_A || e->key() == Qt::Key_Left)
    {
        if(pHead->getDirection()==up_ || pHead->getDirection()==down_)
        pHead->setDirection(left_);
    }
    if (e->key() == Qt::Key_D || e->key() == Qt::Key_Right)
    {
        if(pHead->getDirection()==up_ || pHead->getDirection()==down_)
        pHead->setDirection(right_);
    }

    if (e->key() == Qt::Key_Enter)
    {
        num = ui->lineEdit->text().toInt();
        timerId = this->startTimer(num);//毫秒 每隔一秒触发一次定时器
    }

    update();
}


void Widget::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
    //把QString转换为int
    num = ui->lineEdit->text().toInt();
}
