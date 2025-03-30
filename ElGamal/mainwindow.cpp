#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QToolBar>
#include<QDebug>
#include<QPushButton>
#include<QWidget>
#include"diffiehellman.h"
#include"elgamal.h"
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

void MainWindow::on_pushButton_2_clicked()
{
    qDebug()<<"1";
    this->close();//关闭当前界面，即主界面
    AES*pic1=new AES();
    pic1->setWindowTitle("AES");
    pic1->resize(800,600);
    qDebug()<<"1";


    pic1->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug()<<"6";
    this->close();//关闭当前界面，即主界面
    qDebug()<<"9";
    RSA*pic2=new RSA();
    pic2->setWindowTitle("RSA");
    qDebug()<<"2";
    pic2->resize(800,600);
    if(pic2!=nullptr)
    {

          pic2->show();
    }
    else
    {
      qDebug()<<"pic2异常";
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    this->close();
    DiffieHellman *pic3 = new DiffieHellman();
    pic3->setWindowTitle("Diffie-Hellman");
    pic3->resize(800,600);
    if(pic3!=nullptr)
    {

          pic3->show();
    }
    else
    {
      qDebug()<<"pic3异常";
    }

}

void MainWindow::on_pushButton_5_clicked()
{
    this->close();
    ElGamal *pic4 = new ElGamal();
    pic4->setWindowTitle("ElGamal签名算法");
    pic4->resize(800,600);
    if(pic4!=nullptr)
    {

          pic4->show();
    }
    else
    {
      qDebug()<<"pic4异常";
    }
}
