#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QToolBar>
#include<QDebug>
#include<QPushButton>
#include<QWidget>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //重置窗口大小
    resize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_des_clicked()
{
    this->close();//关闭当前界面，即主界面
    DES*pic=new DES();
    pic->setWindowTitle("DES");
    pic->resize(800,600);
    pic->show();

}
