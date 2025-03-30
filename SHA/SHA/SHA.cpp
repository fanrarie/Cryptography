#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include <windows.h>
#include <sstream>
#include <iomanip>

// �����д���ļ�
void writeResultToFile(const std::string& filename, const std::string& result)
{
    std::ofstream file(filename, std::ios::out | std::ios::binary); // �Զ�����д��ģʽ���ļ�
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filename << std::endl; // ���ʧ����Ϣ
        return;
    }

    file.write(result.data(), result.size()); // д�������ļ�
    file.close(); // �ر��ļ�
}

// ��ʼ�� SHA �ĳ�ʼֵ
uint32_t H0 = 0x67452301;
uint32_t H1 = 0xEFCDAB89;
uint32_t H2 = 0x98BADCFE;
uint32_t H3 = 0x10325476;
uint32_t H4 = 0xC3D2E1F0;

// SHA ���ĸ��ӷ�����
const uint32_t K0 = 0x5A827999;//0<=t<=19
const uint32_t K1 = 0x6ED9EBA1;//20<=t<=39
const uint32_t K2 = 0x8F1BBCDC;//40<=t<=59
const uint32_t K3 = 0xCA62C1D6;//60<=t<=79

// ������SHA ���ܵ�ѭ��
void sround(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t& E, uint32_t W, uint32_t K, int mode);

// �ĸ������Ժ�����ֱ������
inline uint32_t f1(uint32_t B, uint32_t C, uint32_t D); //0<=t<=19
inline uint32_t f2(uint32_t B, uint32_t C, uint32_t D); //20<=t<=39
inline uint32_t f3(uint32_t B, uint32_t C, uint32_t D); //40<=t<=59
inline uint32_t f4(uint32_t B, uint32_t C, uint32_t D); //60<=t<=79

inline uint32_t cirleft(uint32_t word, int bit); // ѭ�����ƺ���

// ��ȡ��Ϣ����
long long msgsize(const std::string& plainaddr); 

// ����ֵת��Ϊʮ�������ַ���
std::string toHexString(uint32_t value); 

int main()
{
    LARGE_INTEGER start, finish, frequency; // ��ʱ
    QueryPerformanceFrequency(&frequency); // ��ȡ��ʱƵ��
    double elapsed = 0; // ��ʱ���

    std::string inputfileaddr = "E:/Qt/EXE(1)/EXE/SHA/input.txt"; // �����ļ���ַ
    std::string outputfile = "E:/Qt/EXE(1)/EXE/SHA/output.txt";   // ����ļ���ַ
    
    uint32_t W[80]; // ���ڽ���ʱ��Ŀռ䣬W ���� 80 ��
    std::memset(W, 0, 64); // ԭʼ�ռ�����
    
    uint32_t A, B, C, D, E; // SHA �㷨�м����
    
    long long msglen = msgsize(inputfileaddr) * 8; // ��ȡԭʼλ���������Ƶ�λ�������ֽڣ�
    std::ifstream file(inputfileaddr, std::ios::binary); // �Զ�����ģʽ���ļ�
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << inputfileaddr << std::endl; // ���ʧ����Ϣ
        system("PAUSE");
        return 1;
    }
    long long counter = 1, times = 0; // �������ʹ���
    int flag = 0; // ��־λ
    int bytes; // ʵ�ʶ�ȡ���ֽ���

    if (msglen % 512 > 440) 
    {
        times = (msglen + 512 - 1) / 512; // ����ֿ����
        flag = 1; // ���ñ�־λ
    }
    else if (msglen % 512 == 0) 
    {
        times = (msglen + 512 - 1) / 512 + 1; 
        flag = 2; 
    }
    else 
        times = (msglen + 512 - 1) / 512; 

    A = H0; B = H1; C = H2; D = H3; E = H4; // ��ʼ�� A��B��C��D��E
    
    while (counter < times) // ѭ��ֱ���ֿ����
    {
        file.read(reinterpret_cast<char*>(W), 64); // ��ȡһ��512λ��Ϣ�鵽 W
        for (int i = 0; i < 16; i++) // ����˳�򣬻�ȡ����Ĵ洢˳��
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        QueryPerformanceCounter(&start); // ��ʱ��ʼ
        for (int i = 0; i < 20; i++) // SHA��һ��
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // ���� SHA ���ܵ�ѭ������
        }

        for (int i = 20; i < 40; i++) // SHA�ڶ���
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // ���� SHA ���ܵ�ѭ������
        }

        for (int i = 40; i < 60; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 60; i < 80; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // ���� SHA���ܵ�ѭ������
        }

        //����A,B,C,D,E
        A = H0 = H0 + A; 
        B = H1 = H1 + B; 
        C = H2 = H2 + C; 
        D = H3 = H3 + D;
        E = H4 = H4 + E; 
        counter++; // ��������1
    }

    if (flag == 1) // �����־λΪ1
    {
        file.read(reinterpret_cast<char*>(W), 64); // ��ȡһ��512λ��Ϣ��
        for (int i = 0; i < 16; i++) // ����˳�򣬻�ȡ����Ĵ洢˳��
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        W[15] = msglen; // ������Ϣ����
        for (int i = 0; i < 16; i++) // ����˳�򣬻�ȡ����Ĵ洢˳��
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        for (int i = 0; i < 20; i++) // SHA��һ��
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 20; i < 40; i++) // SHA�ڶ���
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 40; i < 60; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 60; i < 80; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // ���� SHA-���ܵ�ѭ������
        }

        //����A,B,C,D,E
        A = H0 = H0 + A; 
        B = H1 = H1 + B;
        C = H2 = H2 + C;
        D = H3 = H3 + D; 
        E = H4 = H4 + E; 
    }
    else if (flag == 2) // �����־λΪ2
    {
        file.read(reinterpret_cast<char*>(W), 64); // ��ȡһ��512λ��Ϣ��
        for (int i = 0; i < 16; i++) // ����˳�򣬻�ȡ����Ĵ洢˳��
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        W[15] = 0x80000000; // ������Ϣ����
        for (int i = 16; i < 60; i++) // ��ʼ�� W
        {
            W[i] = 0x00000000;
        }
        W[14] = msglen; // ������Ϣ����
        W[15] = W[15] + msglen; // ������Ϣ����
        for (int i = 0; i < 20; i++) // SHA��һ��
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // ���� SHA ���ܵ�ѭ������
        }

        for (int i = 20; i < 40; i++) // SHA�ڶ���
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 40; i < 60; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 60; i < 80; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // ���� SHA ���ܵ�ѭ������
        }

        //����A,B,C,D,E
        A = H0 = H0 + A; 
        B = H1 = H1 + B; 
        C = H2 = H2 + C; 
        D = H3 = H3 + D; 
        E = H4 = H4 + E; 
    }
    else // ����
    {
        bytes = file.readsome(reinterpret_cast<char*>(W), 64); // ��ȡһ��512λ��Ϣ��
        for (int i = 0; i < bytes / 4; i++) // ����˳�򣬻�ȡ����Ĵ洢˳��
        {
            W[i] = (W[i] >> 24) + (W[i] >> 8 & 0xff00) + (W[i] << 8 & 0xff0000) + (W[i] << 24);
        }
        W[bytes / 4] = 0x80000000; // ������Ϣ����
        for (int i = bytes / 4 + 1; i < 16; i++) // ��ʼ�� W
        {
            W[i] = 0x00000000;
        }
        W[14] = msglen; // ������Ϣ����
        W[15] = W[15] + msglen; // ������Ϣ����
        for (int i = 0; i < 20; i++) // SHA��һ��
        {
            if (i >= 16)
                W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K0, 1); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 20; i < 40; i++) // SHA�ڶ���
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K1, 2); // ���� SHA ���ܵ�ѭ������
        }

        for (int i = 40; i < 60; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K2, 3); // ���� SHA���ܵ�ѭ������
        }

        for (int i = 60; i < 80; i++) // SHA������
        {
            W[i] = ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) << 1) |
                ((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]) >> 31);
            sround(A, B, C, D, E, W[i], K3, 4); // ���� SHA ���ܵ�ѭ������
        }
       
        //����A,B,C,D,E
        A = H0 = H0 + A; 
        B = H1 = H1 + B; 
        C = H2 = H2 + C; 
        D = H3 = H3 + D; 
        E = H4 = H4 + E; 
    }
    std::ofstream output(outputfile, std::ios::binary); 
    
    if (!output.is_open()) // ����ļ���ʧ��
    {
        std::cout << "Failed to create output file: " << outputfile << std::endl; // ���������Ϣ
        system("PAUSE"); 
        return 1; 
    }

    output.write(reinterpret_cast<char*>(&H0), 4); // ��H0д���ļ�
    output.write(reinterpret_cast<char*>(&H1), 4); // ��H1д���ļ�
    output.write(reinterpret_cast<char*>(&H2), 4); // ��H2д���ļ�
    output.write(reinterpret_cast<char*>(&H3), 4); // ��H3д���ļ�
    output.write(reinterpret_cast<char*>(&H4), 4); // ��H4д���ļ�

    std::cout << "SHA hash result has been saved to " << outputfile << std::endl; // ����ɹ���Ϣ

    output.close(); 
    file.close(); 
    std::string result = toHexString(H0) + toHexString(H1) + toHexString(H2) + toHexString(H3) + toHexString(H4); // ����ϣ���ת��Ϊʮ�������ַ���
    writeResultToFile(outputfile, result); // ��ʮ�������ַ���д���ļ�

    std::cout << "SHA calculation completed." << std::endl; // ������������Ϣ

    return 0; 
}

std::string toHexString(uint32_t value)
{
    std::stringstream stream; 
    stream << std::hex << std::setw(8) << std::setfill('0') << value; // ��ʽ��Ϊʮ�������ַ���
    return stream.str(); 
}

//SHA�е��ĸ������Ժ���
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

// ѭ�����ƺ���
inline uint32_t cirleft(uint32_t word, int bit)
{
    return (word << bit) | (word >> (32 - bit)); 
}

long long msgsize(const std::string& plainaddr)
{
    long long counter = 0; // ��ʼ��������
    std::ifstream file(plainaddr, std::ios::binary); 
    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << plainaddr << std::endl; 
        return -1; 
    }
    char ch; // ���ڴ洢��ȡ���ַ�
    while (file.get(ch)) // ѭ����ȡ�ַ�
        counter++; 
    file.close();
    return counter;
}

void sround(uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t& E, uint32_t W, uint32_t K, int mode)
{
    uint32_t TEMP; 
    if (mode == 1) // �����������Ϊ0<=t<=19
    {
        TEMP = cirleft(A, 5) + f1(B, C, D) + E + K + W; // ����SHA���ܵ�ѭ������
    }
    else if (mode == 2) // �����������Ϊ20<=t<=39
    {
        TEMP = cirleft(A, 5) + f2(B, C, D) + E + K + W; 
    }
    else if (mode == 3) // �����������Ϊ40<=t<=59
    {
        TEMP = cirleft(A, 5) + f3(B, C, D) + E + K + W; 
    }
    else if (mode == 4) // �����������Ϊ60<=t<=79
    {
        TEMP = cirleft(A, 5) + f4(B, C, D) + E + K + W; 
    }

    //����A,B,C,D,E
    E = D; 
    D = C; 
    C = cirleft(B, 30); 
    B = A; 
    A = TEMP; 
}
