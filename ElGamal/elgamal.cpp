#include "elgamal.h"
#include "ui_elgamal.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <tuple>
#include <fstream>
#include <sstream>
ElGamal::ElGamal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ElGamal)
{
    ui->setupUi(this);
    setStyleSheet("background-color:#6699CC");
      ui->textBrowser->setStyleSheet("QTextBrowser{color:white;}");
      ui->lineEdit->setStyleSheet("QLineEdit{color:white;}");
      ui->lineEdit_2->setStyleSheet("QLineEdit{color:white;}");

      ui->lineEdit_3->setStyleSheet("QLineEdit{color:white;}");
}

ElGamal::~ElGamal()
{
    delete ui;
}

// (a^b) % mod
long long ElGamal::powmod(long long a, long long b, long long mod)
{
    long long result = 1;
    a = a % mod;
    if (a < 0) a += mod;
    while (b > 0) {
        if (b % 2 == 1) {
            result = (result * a) % mod;
            if (result < 0) result += mod;
        }
        b = b >> 1;
        a = (a * a) % mod;
        if (a < 0) a += mod;
    }
    return result;
}

// 判断素数
bool ElGamal::Prime(long long int a)
{
    if (a <= 1) {
        return false;
    }
    if (a == 2) {
        return true;
    }
    if (a % 2 == 0) {
        return false;
    }
    long long int sqrtNum = sqrt(a);
    for (long long int i = 3; i <= sqrtNum; i += 2) {
        if (a % i == 0) {
            return false;
        }
    }
    return true;
}

// 求a和b的最大公约数
long long int ElGamal::gcd(long long int a, long long int b)
{
    while (b != 0) {
        long long int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


// 生成大素数p
long long int ElGamal::Makep()
{
    srand((unsigned long long int)time(NULL));
    long long int min = 1000000; // 最小素数范围
    long long int max = 10000000; // 最大素数范围

    long long int prime;
    do {
        prime = rand() % (max - min) + min;
    } while (!Prime(prime));
    return prime;
}

// 检查g是否是p的原根
bool ElGamal::is_primitive_root(long long g, long long p)
{
    if ((powmod(g, p - 1, p)) != 1) {
        return false;
    }
    long long int n = p - 1;
    std::set<long long> factors;
    for (long long i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            factors.insert(i);
            n /= i;
        }
    }
    if (n > 1) {
        factors.insert(n);
    }

    for (auto factor : factors) {
        if (powmod(g, (p - 1) / factor, p) == 1) {
            return false;
        }
    }

    return true;
}

// 生成p的本原根g
long long int ElGamal::Getg(long long int p)
{
    if (p == 2) {
        return 1;
    }

    for (long long g = 2; g < p; g++) {
        if (is_primitive_root(g, p)) {
            return g;
        }
    }

    QMessageBox::warning(this, tr("Warning"), tr("没找到本原根！"), QMessageBox::Yes);
    return -1;
}

// 生成X
long long ElGamal::makex(long long int p)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(1, p - 2);
    long long int x;
    x = dis(gen); // 私钥 x
    return x;
}

// 求解乘法逆元
long long int ElGamal::inverse(long long int a, long long int n)
{
    long long t = 0, new_t = 1;
    long long r = n, new_r = a;

    while (new_r != 0) {
        long long quotient = r / new_r;
        std::tie(t, new_t) = std::make_tuple(new_t, t - quotient * new_t);
        std::tie(r, new_r) = std::make_tuple(new_r, r - quotient * new_r);
    }

    if (r > 1) return -1; // a is not invertible
    if (t < 0) t += n; // Ensure t is non-negative

    return t;
}

// 签名算法得到r
long long int ElGamal::signature_r(long long int g, long long int p)
{
    srand((unsigned long long int)time(NULL));
       do {
           k = rand() % (p - 1) + 1; // k 在 1 到 p-1 之间
       } while (gcd(k, p - 1) != 1); // 确保 k 与 p-1 互质
    r = powmod(g, k, p);
    return r;
}

//签名算法得到s
long long int ElGamal::signature_s(long long int m, long long int g, long long int p, long long int x)
{
    long long int k_inv;

    long long int t;

    // 计算 s
        t = m - x * r;
        if (t < 0) t += (p - 1); // 确保 s 为非负数
        k_inv = inverse(k, p - 1);

        if(k_inv<0) k_inv+=(p-1);
        qDebug()<<"k_inv:"<<k_inv;
        long long int tt=k_inv*t;
        s = tt % (p - 1);
        if(s<0) s+=(p-1);
        return s;
}



// 打开文件
void ElGamal::on_pushButton_clicked()
{
    QString fileName = QString::fromStdString(inputfileaddr);
    fileName = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("All Files (*.*)")); // 选择路径
    if (fileName.length() == 0)
    {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        return;
    }
    ui->lineEdit->setText(fileName);
    ui->lineEdit_2->clear();
}

// 保存文件
void ElGamal::on_pushButton_2_clicked()
{
    QString saveName = QString::fromStdString(outputfile);
    saveName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("All Files (*.*);;*.txt")); // 选择路径
    ui->lineEdit_2->setText(saveName);
}

std::vector<long long int> readLongLongFromFile(const std::string& filename)
{
    std::vector<long long int> result;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            try {
                // 尝试将每行内容转换为 long long int
                long long int num = std::stoll(line);
                result.push_back(num);
            } catch (const std::invalid_argument& e) {
                // 转换失败，说明文件中包含非数字字符
                std::cerr << "Error converting line to long long int: " << e.what() << std::endl;
            } catch (const std::out_of_range& e) {
                // 转换失败，说明数字超出了 long long int 的范围
                std::cerr << "Error: Number out of range for long long int: " << e.what() << std::endl;
            }
        }
        file.close();
    } else {
        // 文件打开失败
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    return result;
}

long long int combineVectorToLLInt(const std::vector<long long int>& numbers)
{
    std::stringstream ss;
    for (const auto& num : numbers) {
        ss << num;
    }
    std::string combinedString = ss.str();
    return std::stoll(combinedString); // 将字符串转换为 long long int
}

// 进行签名算法
// 打印所有相关变量的值以进行调试
void ElGamal::on_pushButton_3_clicked()
{

    // 首先将明文使用SHA的哈希函数
    h->hash();
    qDebug() << "1";


    std::vector<long long int> numbers = readLongLongFromFile("E:/Qt/EXE(1)/EXE/output.txt");
    m = combineVectorToLLInt(numbers);
    // 随机生成素数
    p = Makep();
    //p=17;
    qDebug() << "Generated prime (p): " << p;

    // 根据生成的素数计算其原根
    g = Getg(p);
    if (g == -1)
    {
        return;
    }
    qDebug() << "Primitive root (g): " << g;

    // 根据生成的素数得到x
    x = makex(p);
    qDebug() << "Private key (x): " << x;

    // 根据生成的数解出公钥y
    y = powmod(g, x, p);
    qDebug() << "Public key (y): " << y;

    // 将公钥y可视化
    ui->lineEdit_3->setText(QString::number(y));

    // ElGamal签名算法得到r 和s
    r = signature_r(g, p);
    s = signature_s(m, g, p, x);

    // 将签名算法可视化
    QString c = QString::number(r);
    QString d = QString::number(s);
    ui->textBrowser->append("签名生成r:");
    ui->textBrowser->append(c);
    ui->textBrowser->append("\n签名生成s:");
    ui->textBrowser->append(d);
    QMessageBox::information(NULL, tr("Success!"), tr("完成签名生成!"));
}



void ElGamal::on_pushButton_4_clicked()
{
    if (r <= 0 || r >= p || s <= 0 || s >= p - 1)
    {
        QMessageBox::warning(this, tr("Warning"), tr("签名验证失败!"), QMessageBox::Yes);
        return;
    }

    qDebug() << "Verifying signature:";
    qDebug() << "r: " << r << " s: " << s << " p: " << p << " y: " << y << " g: " << g << " m: " << m<<"k:"<<k;

    long long v1 = (powmod(y, r, p) * powmod(r,s, p)) % p;
    long long v2 = powmod(g, m, p);
    qDebug() << "Computed v1: " << v1 << " Computed v2: " << v2;

    QString c = QString::number(v1);
    QString d = QString::number(v2);
    ui->textBrowser->append("\n签名验证v1:");
    ui->textBrowser->append(c);
    ui->textBrowser->append("\n签名验证v2:");
    ui->textBrowser->append(d);
    if (v1 == v2)
    {
        QMessageBox::information(NULL, tr("Success!"), tr("签名验证成功!"));
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("签名验证失败!"), QMessageBox::Yes);
    }
}
