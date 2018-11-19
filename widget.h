#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "snake.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void dealSignal();
    void dealClose();

    void keyPressEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);
    void snakemove();

signals:
    void startThread();//启动子线程的信号

protected:
    void paintEvent(QPaintEvent *);


private slots:
    void on_buttonStart_clicked();
    void on_buttonStop_clicked();


    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::Widget *ui;

    int gridW;
    int gridH;
    int startX;
    int startY;

    Snake * pHead;
    int timerId;
};

#endif // WIDGET_H
