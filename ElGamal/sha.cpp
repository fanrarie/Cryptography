#include "sha.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include <windows.h>
#include <sstream>
#include <iomanip>
SHA::SHA()
{

}


// 将结果写入文件
void writeResultToFile(const std::string& filename, const std::string& result)
{
    std::ofstream file(filename, std::ios::out | std::ios::binary); // 以二进制写入模式打开文件
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filename << std::endl; // 输出失败信息
        return;
    }

    file.write(result.data(), result.size()); // 写入结果到文件
    file.close(); // 关闭文件
}

// 初始化 SHA 的初始值
uint32_t H0 = 0x67452301;
uint32_t H1 = 0xEFCDAB89;
uint32_t H2 = 0x98BADCFE;
uint32_t H3 = 0x10325476;
uint32_t H4 = 0xC3D2E1F0;

// SHA 的四个加法常量
const uint32_t K0 = 0x5A827999;//0<=t<=19
const uint32_t K1 = 0x6ED9EBA1;//20<=t<=39
const uint32_t K2 = 0x8F1BBCDC;//40<=t<=59
const uint32_t K3 = 0xCA62C1D6;//60<=t<=79

// 函数：SHA 加密的循环
void sround(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t& E, uint32_t W, uint32_t K, int mode);

// 四个非线性函数，直接内联
inline uint32_t f1(uint32_t B, uint32_t C, uint32_t D); //0<=t<=19
inline uint32_t f2(uint32_t B, uint32_t C, uint32_t D); //20<=t<=39
inline uint32_t f3(uint32_t B, uint32_t C, uint32_t D); //40<=t<=59
inline uint32_t f4(uint32_t B, uint32_t C, uint32_t D); //60<=t<=79

inline uint32_t cirleft(uint32_t word, int bit); // 循环左移函数

// 获取消息长度
long long msgsize(const std::string& plainaddr);

// 将数值转换为十六进制字符串
std::string toHexString(uint32_t value);

void SHA::hash()
{
    LARGE_INTEGER start, finish, frequency; // 计时
    QueryPerformanceFrequency(&frequency); // 获取计时频率
    double elapsed = 0; // 计时结果

    std::string inputfileaddr = "E:/Qt/EXE(1)/EXE/input.txt"; // 输入文件地址
    std::string outputfile = "E:/Qt/EXE(1)/EXE/output.txt";   // 输出文件地址

    uint32_t W[80]; // 用于交换时间的空间，W 块有 80 个
    std::memset(W, 0, 64); // 原始空间清零

    uint32_t A, B, C, D, E; // SHA 算法中间变量

    long long msglen = msgsize(inputfileaddr) * 8; // 获取原始位数，二进制单位，不是字节！
    std::ifstream file(inputfileaddr, std::ios::binary); // 以二进制模式打开文件
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << inputfileaddr << std::endl; // 输出失败信息
        system("PAUSE");

    }
    long long counter = 1, times = 0; // 计数器和次数
    int flag = 0; // 标志位
    int bytes; // 实际读取的字节数

    if (msglen % 512 > 440)
    {
        times = (msglen + 512 - 1) / 512; // 计算分块次数
        flag = 1; // 设置标志位
    }
    else if (msglen % 512 == 0)
    {
        times = (msglen + 512 - 1) / 512 + 1;
        flag = 2;
    }
    else
        times = (msglen + 512 - 1) / 512;

    A = H0; B = H1; C = H2; D = H3; E = H4; // 初始化 A、B、C、D、E

    while (counter < times) // 循环直到分块次数
    {
        file.read(reinterpret_cast<char*>(W), 64); // 读取一个512位消息块到 W
        for (int i = 0; i < 16; i++) // 调整顺序，获取所需的存储顺序
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        QueryPerformanceCounter(&start); // 计时开始
        for (int i = 0; i < 20; i++) // SHA第一轮
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // 调用 SHA 加密的循环函数
        }

        for (int i = 20; i < 40; i++) // SHA第二轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // 调用 SHA 加密的循环函数
        }

        for (int i = 40; i < 60; i++) // SHA第三轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // 调用 SHA加密的循环函数
        }

        for (int i = 60; i < 80; i++) // SHA第四轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // 调用 SHA加密的循环函数
        }

        //更新A,B,C,D,E
        A = H0 = H0 + A;
        B = H1 = H1 + B;
        C = H2 = H2 + C;
        D = H3 = H3 + D;
        E = H4 = H4 + E;
        counter++; // 计数器加1
    }

    if (flag == 1) // 如果标志位为1
    {
        file.read(reinterpret_cast<char*>(W), 64); // 读取一个512位消息块
        for (int i = 0; i < 16; i++) // 调整顺序，获取所需的存储顺序
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        W[15] = msglen; // 设置消息长度
        for (int i = 0; i < 16; i++) // 调整顺序，获取所需的存储顺序
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        for (int i = 0; i < 20; i++) // SHA第一轮
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // 调用 SHA加密的循环函数
        }

        for (int i = 20; i < 40; i++) // SHA第二轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // 调用 SHA加密的循环函数
        }

        for (int i = 40; i < 60; i++) // SHA第三轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // 调用 SHA加密的循环函数
        }

        for (int i = 60; i < 80; i++) // SHA第四轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // 调用 SHA-加密的循环函数
        }

        //更新A,B,C,D,E
        A = H0 = H0 + A;
        B = H1 = H1 + B;
        C = H2 = H2 + C;
        D = H3 = H3 + D;
        E = H4 = H4 + E;
    }
    else if (flag == 2) // 如果标志位为2
    {
        file.read(reinterpret_cast<char*>(W), 64); // 读取一个512位消息块
        for (int i = 0; i < 16; i++) // 调整顺序，获取所需的存储顺序
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        W[15] = 0x80000000; // 设置消息长度
        for (int i = 16; i < 60; i++) // 初始化 W
        {
            W[i] = 0x00000000;
        }
        W[14] = msglen; // 设置消息长度
        W[15] = W[15] + msglen; // 设置消息长度
        for (int i = 0; i < 20; i++) // SHA第一轮
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // 调用 SHA 加密的循环函数
        }

        for (int i = 20; i < 40; i++) // SHA第二轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // 调用 SHA加密的循环函数
        }

        for (int i = 40; i < 60; i++) // SHA第三轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // 调用 SHA加密的循环函数
        }

        for (int i = 60; i < 80; i++) // SHA第四轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // 调用 SHA 加密的循环函数
        }

        //更新A,B,C,D,E
        A = H0 = H0 + A;
        B = H1 = H1 + B;
        C = H2 = H2 + C;
        D = H3 = H3 + D;
        E = H4 = H4 + E;
    }
    else // 否则
    {
        bytes = file.readsome(reinterpret_cast<char*>(W), 64); // 读取一个512位消息块
        for (int i = 0; i < bytes / 4; i++) // 调整顺序，获取所需的存储顺序
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        W[bytes / 4] = 0x80000000; // 设置消息长度
        for (int i = bytes / 4 + 1; i < 16; i++) // 初始化 W
        {
            W[i] = 0x00000000;
        }
        W[14] = msglen; // 设置消息长度
        W[15] = W[15] + msglen; // 设置消息长度
        for (int i = 0; i < 20; i++) // SHA第一轮
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // 调用 SHA加密的循环函数
        }

        for (int i = 20; i < 40; i++) // SHA第二轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // 调用 SHA 加密的循环函数
        }

        for (int i = 40; i < 60; i++) // SHA第三轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // 调用 SHA加密的循环函数
        }

        for (int i = 60; i < 80; i++) // SHA第四轮
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // 调用 SHA 加密的循环函数
        }

        //更新A,B,C,D,E
        A = H0 = H0 + A;
        B = H1 = H1 + B;
        C = H2 = H2 + C;
        D = H3 = H3 + D;
        E = H4 = H4 + E;
    }
    std::ofstream output(outputfile, std::ios::binary);

    if (!output.is_open()) // 如果文件打开失败
    {
        std::cout << "Failed to create output file: " << outputfile << std::endl; // 输出错误信息
        system("PAUSE");

    }

    output.write(reinterpret_cast<char*>(&H0), 4); // 将H0写入文件
    output.write(reinterpret_cast<char*>(&H1), 4); // 将H1写入文件
    output.write(reinterpret_cast<char*>(&H2), 4); // 将H2写入文件
    output.write(reinterpret_cast<char*>(&H3), 4); // 将H3写入文件
    output.write(reinterpret_cast<char*>(&H4), 4); // 将H4写入文件

    std::cout << "SHA hash result has been saved to " << outputfile << std::endl; // 输出成功信息

    output.close();
    file.close();
    std::string result = toHexString(H0) + toHexString(H1) + toHexString(H2) + toHexString(H3) + toHexString(H4); // 将哈希结果转换为十六进制字符串
    writeResultToFile(outputfile, result); // 将十六进制字符串写入文件

    std::cout << "SHA calculation completed." << std::endl; // 输出计算完成信息


}

std::string toHexString(uint32_t value)
{
    std::stringstream stream;
    stream << std::hex << std::setw(8) << std::setfill('0') << value; // 格式化为十六进制字符串
    return stream.str();
}

//SHA中的四个非线性函数
inline uint32_t f1(uint32_t B, uint32_t C, uint32_t D)
{
    return (B & C) | ((~B) & D);
}

inline uint32_t f2(uint32_t B, uint32_t C, uint32_t D)
{
    return B ^ C ^ D;
}

inline uint32_t f3(uint32_t B, uint32_t C, uint32_t D)
{
    return (B & C) | (B & D) | (C & D);
}

inline uint32_t f4(uint32_t B, uint32_t C, uint32_t D)
{
    return B ^ C ^ D;
}

// 循环左移函数
inline uint32_t cirleft(uint32_t word, int bit)
{
    return (word << bit) | (word >> (32 - bit));
}

long long msgsize(const std::string& plainaddr)
{
    long long counter = 0; // 初始化计数器
    std::ifstream file(plainaddr, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << plainaddr << std::endl;
        return -1;
    }
    char ch; // 用于存储读取的字符
    while (file.get(ch)) // 循环读取字符
        counter++;
    file.close();
    return counter;
}

void sround(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t& E, uint32_t W, uint32_t K, int mode)
{
    uint32_t TEMP;
    if (mode == 1) // 如果迭代步数为0<=t<=19
    {
        TEMP = cirleft(A, 5) + f1(B, C, D) + E + K + W; // 进行SHA加密的循环运算
    }
    else if (mode == 2) // 如果迭代步数为20<=t<=39
    {
        TEMP = cirleft(A, 5) + f2(B, C, D) + E + K + W;
    }
    else if (mode == 3) // 如果迭代步数为40<=t<=59
    {
        TEMP = cirleft(A, 5) + f3(B, C, D) + E + K + W;
    }
    else if (mode == 4) // 如果迭代步数为60<=t<=79
    {
        TEMP = cirleft(A, 5) + f4(B, C, D) + E + K + W;
    }

    //更新A,B,C,D,E
    E = D;
    D = C;
    C = cirleft(B, 30);
    B = A;
    A = TEMP;
}
