#include "mythread.h"
#include <QDebug>

MyThread::MyThread(QObject *parent) : QThread(parent)
{
    is_connected = false;
    paused = false;
    forced_delete = false;
}

void MyThread::pause()
{
    paused = true;
}

void MyThread::resume()
{
    paused = false;
    mutex.lock();
    condition.wakeOne();
    mutex.unlock();
}

void MyThread::run()
{
    for (int i = 0; i < Count; i++)
    {
        if (paused)
        {
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }
        if (forced_delete)
        {
            break;
        }
        emit progress(i);
        QThread::msleep(500);
    }
}
