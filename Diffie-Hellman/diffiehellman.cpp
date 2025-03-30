#include "diffiehellman.h"
#include "ui_diffiehellman.h"
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<QMessageBox>
#include<QPushButton>
#include<QTextBrowser>
#include<QString>
#include<des.h>

using namespace std;

DiffieHellman::DiffieHellman(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiffieHellman)
{
    ui->setupUi(this);
    std::srand(std::time(nullptr));
    setStyleSheet("background-color:#6699CC");
    ui->sushuEdit->setStyleSheet("QLineEdit{color:white;}");
    ui->yuangenEdit->setStyleSheet("QLineEdit{color:white;}");
    ui->textBrowser->setStyleSheet("QTextBrowser{color:white;}");
    ui->textBrowser_2->setStyleSheet("QTextBrowser{color:white;}");
   ui->lineEdit->setStyleSheet("QLineEdit{color:white;}");
   ui->lineEdit_2->setStyleSheet("QLineEdit{color:white;}");
}

DiffieHellman::~DiffieHellman()
{
    delete ui;
}





//计算(a^b)%mod
long long DiffieHellman::powmod(long long a,long long b,long long mod)
{
    long long result = 1;
    a=a%mod;
    while(b>0)
    {
        if(b%2==1)
        {
            result=(a*result)%mod;
        }
        b=b>>1;
        a=(a*a)%mod;
    }
    return result;
}

//在特定范围内随机生成数，用于生成Xa，Xb
long long DiffieHellman::RangeRand(long long a, long long b)
{
    long long temp=0;
    long long tt=0;
    temp=std::rand()%(b-a+1);
    tt=a+temp;
    return tt;
}

//判断是否是素数
bool DiffieHellman::IsPrime(long long a)
{

    if (a <= 1) return false;
    if (a <= 3) return true;
    if (a % 2 == 0 || a % 3 == 0) return false;

    for (int i = 5; i * i <= a; i += 6) {
        if (a % i == 0 || a % (i + 2) == 0) return false;
    }
    return true;
}

//生成私钥
long long DiffieHellman::privateKey(long long p)
{

    QString t=ui->sushuEdit->text();
    p=t.toLongLong();
    long long a=RangeRand(1,p-1);
    return a;


}

//生成公钥
long long DiffieHellman::publicKey(long long g, long long a, long long p)
{
    p = 7;//例子
    QString t=ui->sushuEdit->text();
    p=t.toLongLong();
    g=12;//例子
    QString s=ui->yuangenEdit->text();
    g=s.toLongLong();
    long long A=powmod(g,a,p);
}

//生成共享密钥
long long DiffieHellman::sharedKey(long long A, long long b, long long p)
{
    long long K=powmod(A,b,p);
}


//界面交互
//确认按钮
void DiffieHellman::on_pushButton_clicked()
{
    long long p;
    QString t;
    t=ui->sushuEdit->text();
    p=t.toLongLong();
    if(!IsPrime(p))
    {
        QMessageBox::warning(this,tr("Warning"),tr("该数不是素数，请重新输入！"),QMessageBox::Yes);
        ui->sushuEdit->clear();
        QString t=ui->sushuEdit->text();
        p=t.toLongLong();
    }
    else
    {


        //生成密钥
        long long a,b,g;
        a=privateKey(p);
        b=privateKey(p);

        //生成公钥
        long long A,B;
        A=powmod(g,a,p);
        B=powmod(g,b,p);

        //生成共享密钥
        Ka=powmod(B,a,p);
        Kb=powmod(A,b,p);

        QString c=QString::number(a);
        QString d=QString::number(b);
        QString e=QString::number(A);
        QString f=QString::number(B);
        QString h=QString::number(Ka);
        QString l=QString::number(Kb);

        //输入Alice的对话框
        //std::cout<<"My Secret Key:"<<c<<"\nMy Public Key:"<<e<<"\nThe Computed Shared Key:"<<h;
        ui->textBrowser->append("My Secret Key:");
        ui->textBrowser->append(c);
        ui->textBrowser->append("\nMy Public Key:");
        ui->textBrowser->append(e);
        ui->textBrowser->append("\nThe Public I get from Bob:");
        ui->textBrowser->append(f);
        ui->textBrowser->append("\nThe Computed Shared Key: ");
        ui->textBrowser->append(h);

        //输入Bob的对话框
        ui->textBrowser_2->append("My Secret Key:");
        ui->textBrowser_2->append(d);
        ui->textBrowser_2->append("\nMy Public Key:");
        ui->textBrowser_2->append(f);
        ui->textBrowser_2->append("\nThe Public I get from Alice:");
        ui->textBrowser_2->append(e);
        ui->textBrowser_2->append("\nThe Computed Shared Key: ");
        ui->textBrowser_2->append(l);

        //判断生成的共享密钥是否相等
        if(Ka==Kb)
        {
            QMessageBox::information(NULL, tr("Ka"),tr("Successful!Shared Key is valid."));
            flag=1;
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),tr("Failed！Shared Key is invalid."),QMessageBox::Yes);
            flag=0;
        }
    }

}

//进行Eve攻击


void DiffieHellman::on_pushButton_4_clicked()
{
    long long p;
    QString t;
    t=ui->sushuEdit->text();
    p=t.toLongLong();
    if(!IsPrime(p))
    {
        QMessageBox::warning(this,tr("Warning"),tr("该数不是素数，请重新输入！"),QMessageBox::Yes);
        ui->sushuEdit->clear();
        QString t=ui->sushuEdit->text();
        p=t.toLongLong();
    }
    else
    {


        //生成密钥
        long long a,b,g,i,j;
        //Alice,Bob随机生成私钥
        a=privateKey(p);
        b=privateKey(p);
        //中间人攻击生成私钥
        i=privateKey(p);
        j=privateKey(p);

        //生成公钥
        long long A,B,C,D;
        //Ailce,Bob生成公钥
        A=powmod(g,a,p);
        B=powmod(g,b,p);
        //中间人计算公钥
        C=powmod(g,i,p);//中间人传给Bob
        D=powmod(g,j,p);//中间人传给Alice

        //计算共享密钥
        Ka=powmod(D,a,p);
        Kb=powmod(C,b,p);

        QString c=QString::number(a);
        QString d=QString::number(b);
        QString e=QString::number(A);
        QString f=QString::number(B);
        QString r=QString::number(C);
        QString s=QString::number(D);
        QString h=QString::number(Ka);
        QString l=QString::number(Kb);

        //输入Alice的对话框
        //std::cout<<"My Secret Key:"<<c<<"\nMy Public Key:"<<e<<"\nThe Computed Shared Key:"<<h;
        ui->textBrowser->append("My Secret Key:");
        ui->textBrowser->append(c);
        ui->textBrowser->append("\nMy Public Key:");
        ui->textBrowser->append(e);
        ui->textBrowser->append("\nThe Public I get from Bob:");
        ui->textBrowser->append(s);
        ui->textBrowser->append("\nThe Computed Shared Key: ");
        ui->textBrowser->append(h);

        //输入Bob的对话框
        ui->textBrowser_2->append("My Secret Key:");
        ui->textBrowser_2->append(d);
        ui->textBrowser_2->append("\nMy Public Key:");
        ui->textBrowser_2->append(f);
        ui->textBrowser_2->append("\nThe Public I get from Alice:");
        ui->textBrowser_2->append(r);
        ui->textBrowser_2->append("\nThe Computed Shared Key: ");
        ui->textBrowser_2->append(l);

        //判断生成的共享密钥是否相等
        if(Ka==Kb)
        {
            QMessageBox::information(NULL, tr("Success"),tr("Successful!Shared Key is valid."));
            flag=1;
        }
        else
        {
            QMessageBox::warning(this,tr("Warning"),tr("Failed！Shared Key is invalid."),QMessageBox::Yes);
            flag=0;
        }
    }
}

//确定是否有中间人攻击后，用DES加解密明文
void DiffieHellman::on_pushButton_2_clicked()
{
    if(flag==0)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Man-in-the-Middle Attack!"),QMessageBox::Yes);
    }
    else
    {
        string str1 = "";

        QString inputText = ui->lineEdit->text();

        str1 = inputText.toStdString();
        QString str2 = QString::number(Ka);
        std::string stdStr = str2.toStdString();
        string rs =d->des_encrypt(str1, stdStr);
        QString rt=QString::fromStdString(rs);
        ui->textBrowser_2->append("\nThe encrypted result: ");
        ui->textBrowser_2->append(rt);


    }
}

void DiffieHellman::on_pushButton_3_clicked()
{
    if(flag==0)
    {
        QMessageBox::warning(this,tr("Warning"),tr("Man-in-the-Middle Attack!"),QMessageBox::Yes);
    }
    else
    {
        string str1 = "";

        QString inputText = ui->lineEdit_2->text();

        str1 = inputText.toStdString();
        string str2 = "1111222233334444";

        string rs =d->des_decrypt(str1);
        QString rt=QString::fromStdString(rs);
        ui->textBrowser->append("\nThe encrypted result: ");
        ui->textBrowser->append(rt);


    }
}
