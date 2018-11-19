#include "mythread.h"
#include <QThread>

MyThread::MyThread(QObject *parent) : QObject(parent)
{
    isStop = false;
}

void MyThread::myTimeOut()
{
    while (isStop == false)
    {
        QThread::msleep(100);
        emit mySignal();

        if (true == isStop)
        {
            break;
        }
    }
}

void MyThread::setFlag(bool flag)
{
    isStop = flag;
}

