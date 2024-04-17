#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Threads");

    threads_count = 0, launched_threads = 0;

    ui->pushButton_forcedRemove->setVisible(false);
}

MainWindow::~MainWindow()
{
    clear_mem();
    delete ui;
}

void MainWindow::on_add_threadButton_clicked()
{
    ui->tableWidget->setRowCount(threads_count+1);

    int count = ui->spinBox->value();

    MyThread *thread = new MyThread;
    threads.append(thread);
    thread->Count = count;

    char *letter = new char [16];
    random_ch(letter);
    QTableWidgetItem *item = new QTableWidgetItem("Поток " + QString::fromUtf8(letter));
    ui->tableWidget->setItem(threads_count,0, item);
    delete [] letter;
    letter = nullptr;

    QProgressBar *bar = new QProgressBar(this);
    bar->setMaximum(thread->Count);
    ui->tableWidget->setCellWidget(threads_count,1,bar);

    QTableWidgetItem *new_item = new QTableWidgetItem(QString::number(count));
    ui->tableWidget->setItem(threads_count, 2, new_item);

    connect(thread, SIGNAL(progress(int)), bar, SLOT(setValue(int)));
    threads_count++;
}

void MainWindow::on_pushButton_stopThread_clicked()
{
    int current_thread = ui->tableWidget->currentRow();

    if (current_thread >= 0 && current_thread < ui->tableWidget->rowCount()){
        MyThread *thread = threads.at(current_thread);
        if(thread->isRunning()){
            thread->pause();
            launched_threads--;
            ui->label_num->setText("Количество работающих потоков: " + QString::number(launched_threads));
        }
        else
            QMessageBox::information(this, "Внимание!", "Этот поток уже не активен");
    }
    else{
        QMessageBox::information(this, "Внимание!", "Поток не выбран");
    }
}

void MainWindow::on_pushButton_launch_clicked()
{
    int current_thread = ui->tableWidget->currentRow();

    if (current_thread >= 0 && current_thread <= ui->tableWidget->rowCount()){
        MyThread *thread = threads.at(current_thread);
        if(!thread->isRunning())
        {
            thread->start();
            launched_threads++;
            ui->label_num->setText("Количество работающих потоков: " + QString::number(launched_threads));
            if (thread->is_connected == false){
                connect(thread,SIGNAL(finished()), this, SLOT(thread_finished()));
                thread->is_connected = true;
            }
        }
        else{
            QMessageBox::information(this, "Внимание!", "Нельзя запустить новый поток, если он уже работает!");
        }
    }
    else{
        QMessageBox::information(this, "Внимание!", "Поток не выбран");
    }
}

void MainWindow::on_pushButton_remove_clicked()
{
    int current_thread = ui->tableWidget->currentRow();
    if (current_thread >= 0 && current_thread <= ui->tableWidget->rowCount()){
        MyThread *thread = threads.at(current_thread);
        if ((thread->paused == true || thread->isRunning()) && (!thread->isFinished()))
            QMessageBox::information(this, "Внимание!", "Нельзя удалять работающие потоки!");
        else if (threads_count == 1){
            ui->tableWidget->removeRow(current_thread);
            threads.removeAt(0);
            threads_count--;
            delete thread;
            ui->pushButton_forcedRemove->setVisible(false);
        }
        else{
            threads.removeAt(current_thread);
            ui->tableWidget->removeRow(current_thread);
            threads_count--;
            delete thread;
        }
    }
    else
        QMessageBox::information(this, "Внимание!", "Поток не выбран!");
}

void MainWindow::thread_finished()
{
    launched_threads--;
    ui->pushButton_forcedRemove->setVisible(false);
    ui->label_num->setText("Количество работающих потоков: " + QString::number(launched_threads));
}

void MainWindow::random_ch(char *letter)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    int randomIndex = rand() % 26;
    char randomLetter = 'A' + randomIndex;
    *letter =  randomLetter;
}

void MainWindow::on_pushButton_resume_clicked()
{
    int current_thread = ui->tableWidget->currentRow();
    if (current_thread >= 0 && current_thread <= ui->tableWidget->rowCount()){
        MyThread *thread = threads.at(current_thread);
        if (thread->paused == true){
            thread->resume();
            launched_threads++;
            ui->label_num->setText("Количество работающих потоков: " + QString::number(launched_threads));
        }
        else{
            QMessageBox::information(this, "Внимание!", "Возобновлять нечего!");
        }
    }
    else{
        QMessageBox::information(this, "Внимание!", "Поток нельзя возобновить, осоебнно если он не существует!");
    }
}

void MainWindow::clear_mem()
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++){
        for (int j = 0; j < 2; j++){
            delete ui->tableWidget->item(i,j);
        }
    }
}

void MainWindow::on_pushButton_forcedRemove_clicked()
{
    int current_thread = ui->tableWidget->currentRow();
    MyThread *thread = threads.at(current_thread);
    if (current_thread >= 0 && current_thread <= ui->tableWidget->rowCount()){
        if (threads_count == 1){
            thread->forced_delete = true;
            ui->tableWidget->removeRow(current_thread);
            threads.removeAt(0);
            threads_count--;
            QThread::msleep(1500);
            delete thread;
            ui->pushButton_forcedRemove->setVisible(false);
        }
        else{
            thread->forced_delete = true;
            ui->tableWidget->removeRow(current_thread);
            threads.removeAt(current_thread);
            threads_count--;
            QThread::msleep(1500);
            delete thread;
            ui->pushButton_forcedRemove->setVisible(false);
        }
    }
    else{
        QMessageBox::information(this, "Внимание!", "Поток не выбран!");
        ui->pushButton_forcedRemove->setVisible(false);
    }
}

void MainWindow::on_tableWidget_itemSelectionChanged()
{
    int current_thread = ui->tableWidget->currentRow();
    MyThread *thread = threads.at(current_thread);
    if (thread->isRunning() && thread->paused == false){
        ui->pushButton_forcedRemove->setVisible(true);
    }
    else{
        ui->pushButton_forcedRemove->setVisible(false);
    }
}

