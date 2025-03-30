#pragma once
#ifndef RSA_H
#define RSA_H

#include <QWidget>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <vector>
#include <QString>
#include <ctime>
namespace Ui {
class RSA;
}
//定义大数，满足大素数的要求，并且把符号重载

typedef std::vector<unsigned char> Big_int;

class BigInteger
{
public:
    Big_int value;//记录大数值的大小
    bool flag;//判断数值的正确

    BigInteger() ;
   // ~BigInteger();
    BigInteger(const BigInteger&);
    BigInteger(int);
    BigInteger(std::string &);
    friend QString BigIntegerToStr(BigInteger number);
    BigInteger operator=(const BigInteger&);
    int recover();
    friend std::ostream& operator<<(std::ostream&, const BigInteger&);
    friend bool operator!=(const BigInteger&, const BigInteger&);
    friend bool operator>(const BigInteger&, const BigInteger&);
    friend bool operator<(const BigInteger&, const BigInteger&);
    friend BigInteger operator-(const BigInteger&, const BigInteger&);
    friend BigInteger operator*(const BigInteger&, const BigInteger&);
    friend BigInteger operator/(const BigInteger&, const BigInteger&);
    friend BigInteger operator%(const BigInteger&, const BigInteger&);
    friend BigInteger multimod(BigInteger&, BigInteger&, BigInteger&);//重载（a*b）mod
    friend BigInteger powmod(BigInteger&, BigInteger&, BigInteger&);//重载(a^b) % mod

};

const BigInteger z(0);//给出大数0的定义
const BigInteger o(1);//给出大数1的定义
const BigInteger t(2);//给出大数2的定义
const int length = 8;
const int pro = 5;

class RSA : public QWidget
{
    Q_OBJECT

public:
    explicit RSA(QWidget *parent = nullptr);
    ~RSA();
    QString h;
    void initKeys();
    void makeKey(BigInteger&, BigInteger&);
    void encrypt(BigInteger & , BigInteger& );//加密
    void encryption(BigInteger &, BigInteger &,BigInteger & exp,BigInteger & mod);
    void decrypt(BigInteger&, BigInteger&);//解密
private slots:
    void on_RSAjia_clicked();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_RSAjie_clicked();

private:

    BigInteger euclid(const BigInteger&, const BigInteger&);

    void Random(BigInteger &, int);

    void RangeRandom(BigInteger &, const BigInteger &);

    bool IsPrime(BigInteger &);

    void MakePrime(BigInteger &);
    BigInteger e;
    BigInteger d;
    BigInteger n;
     BigInteger crypt;
       bool flag1;
    Ui::RSA *ui;
};




#endif // RSA_H
