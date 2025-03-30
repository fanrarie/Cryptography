#include "mainwindow.h"
#include <QApplication>
#include<QLabel>
#include<QVBoxLayout>
#include<QLineEdit>
#include<QPushButton>
#include<iostream>
#include<QMenu>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("Cipher");
    w.show();

    return a.exec();
}
