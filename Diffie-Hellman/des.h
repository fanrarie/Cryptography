#ifndef DES_H
#define DES_H

#include <QWidget>
#include<iostream>
using namespace std;

namespace Ui
{
class DES;
}

class DES : public QWidget
{
    Q_OBJECT

public:
    explicit DES(QWidget *parent = nullptr);
    ~DES();
    string final_permutation(string s);
    string P_box(string s);
    string S_box(string s);
    string desXOR(string s1, string s2);
    string secret_key_compression_replacement(string s);
    string secret_ket_left_move(int k, string s);//密钥循环左移k位
    string secret_key_initial_permutation(string s);
    string plaintext_initial_permutation(string s);
    string des_H(string s);
    string plaintext_righthalf_extended_permutation(string s);
    string des_G(string str);
    string des_f(string str1, string str2);
    void des_generateKeys(string s);
    string des_StrToBitStr(string str);
    string des_BitStrToStr(string bstr);
    string chardeel(string& str1, string& str2);
    string des_encrypt(string str1, string str2);
    string des_decrypt(string str);
    bool onLineEdit2TextChanged(const QString &text);
    void onBackButtonClicked();
signals:
    void backButtonClicked();
private slots:

    void on_DESjia_clicked();

    void on_back_clicked();

    void on_DESjie_clicked();

private:
    Ui::DES *ui;
};

#endif // DES_H
