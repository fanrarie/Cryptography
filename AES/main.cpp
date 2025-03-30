#include "mainwindow.h"
#include <QApplication>
#include<QLabel>
#include<QVBoxLayout>
#include<QLineEdit>
#include<QPushButton>
#include<iostream>
#include<QMenu>
#include<QTextCodec>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if(QT_VERSION<=QT_VERSION_CHECK(5,0,0))
#if_MSC_VER
    QTextCodec *codec=QTextCodec::codecForName("GBK");
#else
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
#endif
    QTextCodec::setCodecForLocale(codec);


    MainWindow w;
    w.setWindowTitle("Cipher");
    w.show();

    return a.exec();
}
