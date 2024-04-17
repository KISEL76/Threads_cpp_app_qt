#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

    int Count;

    void pause();

    void resume();

    bool paused;

    bool is_connected;

    bool forced_delete;

signals:
    void progress(int value);


protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;
};

#endif // MYTHREAD_H
