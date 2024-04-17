#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mythread.h"


#include <QMainWindow>
#include <QProgressBar>
#include <QMessageBox>
#include <QList>
#include <ctime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_add_threadButton_clicked();

    void on_pushButton_stopThread_clicked();

    void on_pushButton_launch_clicked();

    void on_pushButton_remove_clicked();

    void thread_finished();

    void random_ch(char *letter);

    void on_pushButton_resume_clicked();

    void clear_mem();

    void on_pushButton_forcedRemove_clicked();

    void on_tableWidget_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    MyThread *thread_1;
    QList<MyThread*> threads;
    int threads_count;
    int launched_threads;
};
#endif // MAINWINDOW_H
