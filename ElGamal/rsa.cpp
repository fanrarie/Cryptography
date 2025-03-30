#include "rsa.h"
#include "ui_rsa.h"
#include <QString>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <stddef.h>
#include<QDebug>
using namespace std;

BigInteger::BigInteger()
{
    flag=true;


}

//大数的定义
BigInteger::BigInteger(const BigInteger& number)
{
    value = number.value;
    flag = number.flag;
}

BigInteger::BigInteger(int number)
{
    if (number == 0)
    {
        value.push_back(0);//值为0
        flag = true;
    }
    else
    {
        //因为大素数要大于0，所有大数小于0时不成立
        if (number < 0)
        {
            flag = false;
            number *= (-1);
        }
        else
        {
            flag = true;
        }
        while (number != 0)
        {
            value.push_back(number % 10);//把每一位数一个一个存入value中
            number /= 10;
        }
    }
}

BigInteger::BigInteger(std::string &x)
{
    int size = x.size();
    for (int i = size - 1; i >= 0; i--) {
        value.push_back(x[i] - '0');
    }
    flag = true;
}



//将int转为string类型
QString BigIntegerToStr(BigInteger number)
{

   QString result;

  for(int i=number.value.size()-1; i>=0; i--)
  {
      result+=QString::number(number.value[i],10);
  }


   return result;
}


//将赋值重载
BigInteger BigInteger::operator=(const BigInteger& number){

    if (this == &number)
        return *this;

    flag = number.flag;
    this->value.resize(number.value.size());
    for(int i=0;i<number.value.size();i++)
        this->value[i]=number.value[i];
    return *this;
}

//将大数的值还原
int BigInteger::recover()
{
if (value.size()>8)
    return 0;
else
{
    int result=0;
    for (int i=value.size()-1;i>=0;i--)
    {
    result*=10;
    result+=value[i];
    }
    return result;
}

}
#pragma region DATA
int cmp(const Big_int &a, const Big_int &b) {
    //qDebug()<<a.size();

    if (a.size() != b.size()) {
        // 如果两个数字的长度不同，则较长的数字更大
        return a.size() > b.size() ? 1 : -1;
    }
    // 从最高位开始比较
    for (int i = a.size() - 1; i >= 0; --i) {
        if (a[i] != b[i]) {
            // 如果在任何位上数字不同，则根据哪个数字在该位上较大来决定结果
            return a[i] > b[i] ? 1 : -1;
        }
    }
    // 所有位都相同，返回0
    return 0;
}

//重载各个符号,
//重载小于号
bool operator < (const Big_int &a, const Big_int&b)
{
    return cmp(a, b) < 0;
}

//重载大于号
bool operator > (const Big_int &a, const Big_int &b)
{
    return cmp(a, b) > 0;
}

//重载等于号
bool operator == (const Big_int&a, const Big_int &b)
{
    return cmp(a, b) == 0;
}

//重载不等号
bool operator != (const Big_int &a, const Big_int &b)
{
    return cmp(a, b) != 0;
}

//重载小于等于号
bool operator <= (const Big_int &a, const Big_int &b)
{
    return cmp(a, b) <= 0;
}

//重载大于等于号
bool operator >= (const Big_int &a, const Big_int &b) {
    return cmp(a, b) >= 0;
}

//重载加号
Big_int operator+(const Big_int& left, const  Big_int& right)
{
    Big_int result;
    int left_size = left.size(),
        right_size = right.size();

    int i = 0;
    while (i < left_size || i < right_size)
    {
        result.push_back((i < left_size ? left[i] : 0) + (i < right_size ? right[i] : 0));
        i++;
    }

//计算result
    for (i = 0; i + 1 != result.size(); i++)
    {
        result[i + 1] += result[i] / 10;
        result[i] %= 10;
    }


    if (result[i] >= 10) {
        result.push_back(result[i] / 10);
        result[i] %= 10;
    }
    return result;
}

//重载减号
Big_int operator-(const Big_int& left, const Big_int& right)
{
    if (left < right) {
        throw std::domain_error("Cannot subtract a larger number from a smaller one without negative support.");
    }

    if (left == right) {
        return Big_int(0); // 如果两数相等，直接返回0
    }

    Big_int result;
    result.resize(left.size(), 0);

    int carry = 0;
    for (int i = 0; i < left.size(); i++) {
        int sub = left[i] - (i < right.size() ? right[i] : 0) - carry;
        carry = 0;
        if (sub < 0) {
            sub += 10;
            carry = 1;
        }
        result[i] = sub;
    }

    // 移除结果的前导零
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    return result;
}


//重载乘号
Big_int operator*(const Big_int& left, const Big_int& right){
    Big_int result;
    result.resize(left.size() + right.size(), 0); // +1 是多余的
    for (int i = 0; i < left.size(); ++i) {
        int carry = 0; // 引入进位变量
        for (int j = 0; j < right.size() || carry; ++j) {
            int current = result[i + j] + carry;
            if (j < right.size()) current += left[i] * right[j];
            carry = current / 10;
            result[i + j] = current % 10;
        }
    }

    // 移除前导零
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    return result;
}


//重载除号
Big_int operator/(const Big_int& left, const Big_int& right)
{
    Big_int result;
    qDebug()<<left.size();
    if (right.size() == 1 && right[0] == 0)
    {
        throw std::invalid_argument("Division by zero exception in Big_int division.");
    }
    qDebug()<<"a";

    if (left < right) {
        qDebug()<<"shuzu";
        return Big_int(0);  // 如果被除数小于除数，直接返回0
        qDebug()<<"shuzu77";
    }
    qDebug()<<">";


    Big_int a = left, b = right;

    int x = 0;
    while (a >=b)
    {
                b.insert(b.begin(), 0);
        x++;
    }

        b.erase(b.begin());

    Big_int edinica;
    edinica.push_back(1);

    while (x--) {
        while (a >= b)
        {
            a = a - b;
            result = result + edinica;
        }
                result.insert(result.begin(),0);
                b.erase(b.begin());
    }

        result.erase(result.begin());
    if (result.size() == 0)
        result.push_back(0);
    return result;
}
//重载取余
Big_int operator%(const Big_int& left, const  Big_int& right)
{

    if (left < right)
        return left;

    int x = 0;
    Big_int a = left, b = right;
    while (a >= b)
    {
        b.insert(b.begin(), 0);
        x++;
    }

        b.erase(b.begin());

    while (x--)
    {
        while (a >= b)
        {
            a = a - b;
        }
                b.erase(b.begin());
    }
    return a;
}

#pragma endregion


bool operator==(const BigInteger& left, const BigInteger& right)
{
    if (left.flag != right.flag)
        return false;
    else
    {


        if (left.value==right.value)
        {  qDebug()<<"=";
            return true;}
        else {
            return false;
        }

    }

}

bool operator!=(const BigInteger& left, const BigInteger& right)
{
    return !(left == right);
}

bool operator<(const BigInteger& left, const BigInteger& right)
{
    qDebug()<<"123";
    if (left.flag != right.flag)
    {
        if (left.flag == true)
            return false;
        else
            return true;
    }
    if (left.flag == true)
    {
        if (left.flag < right.flag)
            return true;
        else
            return false;
    }

    if (left.flag == false)
    {
        if (left.flag>right.flag)
            return true;
        else
            return false;
    }
}

bool operator>(const BigInteger& left, const BigInteger& right)
{
    return (right < left);
}

//重载输出
ostream& operator<<(ostream& out, const BigInteger& number)
{
    qDebug()<<"os";
    if (!number.flag)
        out << "-";
    for (int i = number.value.size() - 1; i >= 0; i--){

        out <<(int) number.value[i];
    }
    return out;
}

//重载加号
BigInteger operator+(const BigInteger& left, const BigInteger& right){
    BigInteger result;
    if (left.flag != right.flag)
    {
        if (left.flag)
        {
            if (right.value == left.value)
            {
                return z;//当正数负数绝对值相等时为0
            }
            if (right.value < left.value)
            {
                result.flag = true;
                result.value = left.value - right.value;
                return result;
            }
            else
            {
                result.flag = false;
                result.value = right.value - left.value;
                return result;
            }
        }
        else
        {
            if (right.value == left.value)
            {
                return z;
            }
            if (right.value > left.value)
            {
                result.flag = true;
                result.value = right.value - left.value;
                return result;
            }
            else
            {
                result.flag = false;
                result.value = left.value - right.value;
                return result;
            }
        }
    }
    else
    {
        if (!left.flag)
        {
            result.flag = false;
            result.value = left.value + right.value;
            return result;
        }
        if (left.flag)
        {
            result.flag = true;
            result.value = left.value + right.value;
            return result;
        }
    }

}

//重载减号
BigInteger operator-(const BigInteger& left, const BigInteger& right){
    BigInteger result = BigInteger(0);
    if (left.flag == right.flag)
    {

        if (left.value == right.value)
        {
            return z;
        }
        if (left.flag)
        {

            if (left.value > right.value)
            {
                   qDebug()<<"-";
                result.flag = true;
                result.value = left.value - right.value;
                return result;
            }
            else
            {
                result.flag = false;
                result.value = right.value - left.value;
                return result;
            }
        }
        else
        {
            if (left.value > right.value)
            {
                   qDebug()<<"-";
                result.flag = false;
                result.value = left.value - right.value;
                return result;
            }
            else
            {
                result.flag = true;
                result.value = right.value - left.value;
                return result;
            }
        }
    }
    else
    {
        if (left.flag)
        {
            result.flag = true;
            result.value = left.value + right.value;
            return result;
        }
        else
        {
            result.flag = false;
            result.value = left.value + right.value;
            return result;
        }
    }
}

//重载乘号
BigInteger operator*(const BigInteger& left, const BigInteger& right)
{
    BigInteger result;
    if (left.flag != right.flag)
        result.flag = false;
    else
        result.flag = true; // 需要处理符号的一致性

    // 模拟大数乘法，类似于下面的 Big_int 类实现
    result.value.resize(left.value.size() + right.value.size(), 0);
    for (size_t i = 0; i < left.value.size(); ++i) {
        for (size_t j = 0; j < right.value.size(); ++j) {
            result.value[i + j] += left.value[i] * right.value[j];
            result.value[i + j + 1] += result.value[i + j] / 10;
            result.value[i + j] %= 10;
        }
    }

    // 移除前导零
    while (result.value.size() > 1 && result.value.back() == 0) {
        result.value.pop_back();
    }


    return result;

}


BigInteger operator/(const BigInteger& left, const BigInteger& right){
    BigInteger result;

    if (left.flag != right.flag)
        result.flag = false;
qDebug()<<"2";
cout<<left;
    result.value = left.value / right.value;
    qDebug()<<"chufa";
    return result;
}

BigInteger operator%(const BigInteger& left, const BigInteger& right)
{

    BigInteger result = BigInteger(0);
   qDebug()<<z.value[0];
    if (right == z)
    {  // 增加对除数为0的处理，防止运行时错误
        qDebug()<<right.value.size();
        qDebug()<<"出错";
        throw std::runtime_error("Division by zero exception in BigInteger modulus operation.");
    }

    result.value = left.value % right.value;  // 确保value支持取余操作
     qDebug()<<"j";
    return result;
}


 //重载（a*b）mod
BigInteger multimod(BigInteger &a, BigInteger &b, BigInteger &modul)
{
    if (b == z)
        return o;
    if (b == o)
        return a;
    BigInteger tr,su;
    tr = b % t;
    if (tr == z)
    {
        su = b / t;
        tr = multimod(a, su, modul);
        su = tr*t;
        su = su%modul;
        return su;
    }
    su = b - o;
    tr = multimod(a, su, modul);
    su = tr + a;
    su = su%modul;
    return su;
}

//(a^b) % mod
/*BigInteger powmod(BigInteger &num, BigInteger &power, BigInteger &modulus){
    qDebug()<<"P";
    if (power == z)
        return o;
    if (power == o)
        return num;

    BigInteger tr, su;

    tr = power % t;

    if (tr == z)
    {
        su = power / t;
        tr = powmod(num, su, modulus);
        su = multimod(tr, tr, modulus);
        su = su%modulus;
        return su;
    }
    su = power - o;

    tr = powmod(num, su, modulus);
     qDebug()<<"q";
    su =multimod(tr, num, modulus);
    su = su%modulus;
    return su;
}*/
BigInteger powmod(BigInteger &base, BigInteger &exp, BigInteger &mod) {
    BigInteger result=base;
    qDebug() << "Entering powmod with base size = " << base.value.size() << ", exp size = " << exp.value.size();
    if (exp == BigInteger(0)) {  // 检查指数是否为0
        return BigInteger(1);  // 任何数的0次幂定义为1（包括0^0）
    }

    if (base == BigInteger(0)) {
        return BigInteger(0);  // 0的任何正指数次幂都是0
    }
  BigInteger i=BigInteger(0);
    while(i<exp)
    {
        result=result*result;
    }
    result=result%mod;
    return result;
}




BigInteger RSA::euclid(const BigInteger& exp, const BigInteger& mod)
{
    BigInteger a=BigInteger(exp);
   BigInteger b=BigInteger(mod);
   qDebug()<<"b:"<<b.value.size();
    BigInteger x1=BigInteger(0);
    BigInteger x2=BigInteger(1);
    BigInteger y1=BigInteger(1);
    BigInteger y2=BigInteger(0);
    BigInteger q =BigInteger(0);
    BigInteger r=BigInteger(0);
   BigInteger x=BigInteger(0);
   BigInteger y=BigInteger(0);

    while (b != z) // 辗转相除
    {
        qDebug()<<"xci";
        q = a / b;
        qDebug()<<"2ci";
        r = a - q * b;
        qDebug()<<"3ci";
        x = x2 - q * x1;
        qDebug()<<"4ci";
        y = y2 - q * y1;
        qDebug()<<"5ci";
        a = b;
        qDebug()<<"6ci";
        b = r;
        qDebug()<<"7ci";
        x2 = x1;
        qDebug()<<"8ci";
        x1 = x;
        qDebug()<<"9ci";
        y2 = y1;
        qDebug()<<"0ci";
        y1 = y;
    }

    if (x2 < z)
        x2 = x2 + mod;

    return x2;
}



void RSA::Random(BigInteger &num, int Digits)
{
    srand(time(NULL));  // 设置随机种子
    num = BigInteger(0);  // 确保从0开始
   // qDebug()<<"初始"<<num.value.size();
    for (int i = 0; i < Digits; i++)
    {

        int digit = rand() % 10;  // 生成一个0到9之间的数字
        //qDebug()<<digit;



        num = num* BigInteger(10) + BigInteger(digit);   // 将数字添加到num的最低位

        //qDebug()<<"长度"<<num.value.size();
    }
}


//随机生成大数
void RSA::RangeRandom(BigInteger &num, const BigInteger &top)
{

    int Digit = (rand() % (length-1)) + 1;//不超过length-1
    Random(num, Digit);
}

//判断是否为素数
bool RSA::IsPrime(BigInteger &num)
{

    BigInteger num_minione = BigInteger(0);
    qDebug()<<"num:"<<num.value.size();
    num_minione = num - o;
qDebug()<<"num_minione:"<<num_minione.value.size();
    BigInteger temp = BigInteger(0);
    BigInteger tr =BigInteger(0) ;
    BigInteger random = BigInteger(0);
    BigInteger mod = BigInteger(0);
    BigInteger su=BigInteger(0);
    tr = num_minione;
    qDebug()<<"tr:"<<tr.value.size();

    temp = num_minione%t;

    while (temp == z)
    {
        tr = tr / t;
        temp = tr%t;
    }

    for (int i = 0; i < pro; i++)
    {
        temp = tr;

        RangeRandom(random, num_minione);

        mod = powmod(random, temp, num);
qDebug()<<"d";
        while (temp != num_minione && mod != o && mod != num_minione)
        {

            mod = multimod(mod, mod, num);
            temp = temp*t;
        }
        su = temp%t;

        if (mod != num_minione && su == z)
        {

            return false;
        }

    }
    return true;
}

//随机生成大素数
void RSA::MakePrime(BigInteger &num)
{

    bool su = false;

    while (!su)
    {

        Random(num, length);
         qDebug()<<"y";
        su = IsPrime(num);
        qDebug()<<"c";
    }
}

//加密
void RSA::encrypt(BigInteger & input, BigInteger& encrypted)
{
    encrypted = powmod(input, e, n);
}

void RSA::encryption(BigInteger &  input, BigInteger & encrypted,BigInteger & exp,BigInteger & mod)
{
encrypted = powmod(input, exp, mod);

}


void RSA::decrypt(BigInteger& out, BigInteger& encrypted)
{
    out = powmod(encrypted, d, n);
}

void RSA::makeKey(BigInteger & a, BigInteger& b)
{
    a = e;
    b = n;
}
RSA::RSA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RSA)
{
    ui->setupUi(this);

setStyleSheet("background-color:balck");
ui->textEdit->setStyleSheet("QTextEdit{color:white;background-color:black;}");
ui->textBrowser->setStyleSheet("QTextBrowser{color:white;background-color:black;}");

    initKeys();  // 初始化密钥的函数
}

void RSA::initKeys()
{
    BigInteger p=BigInteger(0);
    BigInteger q=BigInteger(0);
            BigInteger  phi= BigInteger(0);
 qDebug()<<"XXX";

    MakePrime(p);
     qDebug()<<"XXX";
    MakePrime(q);
        phi = (p - o) * (q - o);

        n = p * q;

    e = (BigInteger)65537;
qDebug()<<"e:"<<e.value.size();
    d = euclid(e, phi);
    qDebug()<<"lll";
}


RSA::~RSA()
{
    delete ui;
}

void RSA::on_RSAjia_clicked()
{
    QString l,r,s;
    h=ui->textEdit->toPlainText();
    qDebug()<<"h:"<<h;
    std::string k;
    k=h.toLocal8Bit().constData();
    qDebug()<<"k:"<<k.size();
    if (flag1 == false)
    {
        makeKey(e, n);
        l = BigIntegerToStr(e);
        ui->lineEdit->setText(l);

        s= BigIntegerToStr(n);
        ui->lineEdit2->setText(s);


        for(int i=0; i<h.length();i++)
        {
            BigInteger message(k[i]);

            encrypt(message, crypt);
            r += BigIntegerToStr(crypt);
            r += ";";
        }

    }

    else
    {
        std::string ee,nn;
        QString eee= ui->lineEdit->text();
        QString nnn=ui->lineEdit2->text();

        ee=eee.toStdString();
        nn=nnn.toStdString();
        BigInteger e(ee);
        BigInteger n(nn);
        for(int i=0; i<h.length();i++)
        {
            BigInteger message(k[i]);

            encryption(message, crypt,e,n);
            r += BigIntegerToStr(crypt);
            r += ";";
        }
    }
    ui->textBrowser->setPlainText(r);
}



void RSA::on_radioButton_toggled(bool checked)
{
    flag1 = false;
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit2->setReadOnly(true);
}

void RSA::on_radioButton_2_toggled(bool checked)
{
    flag1 = true;

    QString a,c;
    std::string b,d;

    ui->lineEdit->setReadOnly(false);
    a= ui->lineEdit->text();
    b = a.toStdString();
    e = BigInteger(b);

    ui->lineEdit2->setReadOnly(false);
    c = ui->lineEdit2->text();
    d = c.toStdString();
    n =BigInteger(d);
}

void RSA::on_RSAjie_clicked()
{
    QString l;
    h=ui->textEdit->toPlainText();
       std::string k,temp_saver,total;

       k=h.toLocal8Bit().constData();
       vector<string> saver;
       temp_saver.clear();
       for(int i=0; i<k.size();i++)
       {
       if (k[i]!=';')
           temp_saver +=k[i];
       else
       {
           saver.push_back(temp_saver);
           temp_saver.clear();
       }
       }
       BigInteger result=BigInteger(0);
       l.clear();
       total.clear();
       for(int i=0; i<saver.size();i++)
       {
           BigInteger message(saver[i]);
           decrypt(result, message);
           total += result.recover();

       }
       l=QString::fromStdString(total);
       ui->textBrowser->setText(l);
   }


