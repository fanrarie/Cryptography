#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <QWidget>
#include<sha.h>
namespace Ui {
class ElGamal;
}

class ElGamal : public QWidget
{
    Q_OBJECT

public:
    explicit ElGamal(QWidget *parent = nullptr);
    ~ElGamal();
    long long powmod(long long a,long long b,long long mod);
   bool Prime(long long int a);
   long long int gcd(long long int a,long long int b);
   long long int order(long long int a,long long int n,long long int b);
   long long int Makep();
   bool is_primitive_root(long long g, long long p);
   long long int Getg(long long int p);
   long long int makex(long long int p);
   long long int inverse(long long int a,long long int b);
   long long int signature_r(long long int g,long long int p);
   long long int signature_s(long long int m,long long int g,long long int p,long long int x);
 SHA *h;
   private slots:
       void on_pushButton_clicked();

       void on_pushButton_2_clicked();

       void on_pushButton_3_clicked();

       void on_pushButton_4_clicked();

private:
    Ui::ElGamal *ui;
    std::string inputfileaddr;
    std::string outputfile;
    long long int p,g,y,x,r,s,m,k;

};

class SHA;
#endif // ELGAMAL_H
