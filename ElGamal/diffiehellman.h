#ifndef DIFFIEHELLMAN_H
#define DIFFIEHELLMAN_H

#include <QWidget>
#include<des.h>
namespace Ui {
class DiffieHellman;
}

class DiffieHellman : public QWidget
{
    Q_OBJECT

public:
    explicit DiffieHellman(QWidget *parent = nullptr);
    ~DiffieHellman();

long long powmod(long long,long long, long long);//计算(a^b)%mod
long long RangeRand(long long, long long );//在一定范围内随机生成数
bool IsPrime(long long);//判断素数
long long privateKey(long long);//生成私钥
long long publicKey(long long,long long,long long);//生成公钥
long long sharedKey(long long,long long,long long);//生成共享密钥
DES *d;
private slots:
void on_pushButton_clicked();

void on_pushButton_4_clicked();

void on_pushButton_2_clicked();

void on_pushButton_3_clicked();

private:
    Ui::DiffieHellman *ui;
    int flag=0;//判断密钥交换是否成功
    long long Ka;
    long long Kb;

};
class DES;


#endif // DIFFIEHELLMAN_H
