#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTimer>
QTimer *timer1=new QTimer();
QTimer *timer2=new QTimer();


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(timer1,
            SIGNAL(timeout()),
            this,
            SLOT(incrementar_1())
            );
    connect(timer2,
            SIGNAL(timeout()),
            this,
            SLOT(incrementar_2())
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::incrementar_1()
{
    int aux;
    aux=ui->progressBar_1->value();
    aux++;
    ui->progressBar_1->setValue((aux));
}
void MainWindow::incrementar_2()
{
    int aux;
    aux=ui->progressBar_2->value();
    aux++;
    ui->progressBar_2->setValue((aux));
}


void MainWindow::on_pushButton_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    timer1->start(100);

}

void MainWindow::on_pushButton_2_clicked()
{
    close();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    timer2->start(100);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
