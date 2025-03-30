
#ifndef __RIJNDAEL_H__
#define __RIJNDAEL_H__
#include <QException>
#include <QString>

using namespace std;


class Rijndael
{
public:

    enum { ECB=0, CBC=1, CFB=2 };

private:
    enum { DEFAULT_BLOCK_SIZE=16 };
    enum { MAX_BLOCK_SIZE=32, MAX_ROUNDS=14, MAX_KC=8, MAX_BC=8 };

    static int Mul(int a, int b)
    {
        return (a != 0 && b != 0) ? sm_alog[(sm_log[a & 0xFF] + sm_log[b & 0xFF]) % 255] : 0;
    }

    //Convenience method used in generating Transposition Boxes
    static int Mul4(int a, char b[])
    {
        if(a == 0)
            return 0;
        a = sm_log[a & 0xFF];
        int a0 = (b[0] != 0) ? sm_alog[(a + sm_log[b[0] & 0xFF]) % 255] & 0xFF : 0;
        int a1 = (b[1] != 0) ? sm_alog[(a + sm_log[b[1] & 0xFF]) % 255] & 0xFF : 0;
        int a2 = (b[2] != 0) ? sm_alog[(a + sm_log[b[2] & 0xFF]) % 255] & 0xFF : 0;
        int a3 = (b[3] != 0) ? sm_alog[(a + sm_log[b[3] & 0xFF]) % 255] & 0xFF : 0;
        return a0 << 24 | a1 << 16 | a2 << 8 | a3;
    }

public:
    //CONSTRUCTOR
    Rijndael();

    //DESTRUCTOR
    virtual ~Rijndael();


    void MakeKey(char const* key, char const* chain, int keylength=DEFAULT_BLOCK_SIZE, int blockSize=DEFAULT_BLOCK_SIZE);

private:

    void Xor(char* buff, char const* chain)
    {
        if(false==m_bKeyInit)
            throw QString(sm_szErrorMsg1);

        for(int i=0; i<m_blockSize; i++)
            *(buff++) ^= *(chain++);
    }

    void DefEncryptBlock(char const* in, char* result);
    void DefDecryptBlock(char const* in, char* result);

public:
    void EncryptBlock(char const* in, char* result);

    void DecryptBlock(char const* in, char* result);

    void Encrypt(char const* in, char* result, size_t n, int iMode=ECB);

    void Decrypt(char const* in, char* result, size_t n, int iMode=ECB);

    int GetKeyLength()
    {
        if(false==m_bKeyInit)
            throw QString(sm_szErrorMsg1);

        return m_keylength;
    }


    int	GetBlockSize()
    {
        if(false==m_bKeyInit)
            throw QString(sm_szErrorMsg1);

        return m_blockSize;
    }

    //计算轮数
    int GetRounds()
    {
        if(false==m_bKeyInit)
            throw QString(sm_szErrorMsg1);

        return m_iROUNDS;
    }

    void ResetChain()
    {
        memcpy(m_chain, m_chain0, m_blockSize);
    }

public:

    static char const* sm_chain0;

private:
    static const int sm_alog[256];
    static const int sm_log[256];
    static const char sm_S[256];
    static const char sm_Si[256];
    static const int sm_T1[256];
    static const int sm_T2[256];
    static const int sm_T3[256];
    static const int sm_T4[256];
    static const int sm_T5[256];
    static const int sm_T6[256];
    static const int sm_T7[256];
    static const int sm_T8[256];
    static const int sm_U1[256];
    static const int sm_U2[256];
    static const int sm_U3[256];
    static const int sm_U4[256];
    static const char sm_rcon[30];
    static const int sm_shifts[3][4][2];

    static char const* sm_szErrorMsg1;
    static char const* sm_szErrorMsg2;

    bool m_bKeyInit;

    int m_Ke[MAX_ROUNDS+1][MAX_BC];

    int m_Kd[MAX_ROUNDS+1][MAX_BC];

    int m_keylength;

    int	m_blockSize;
    //轮数
    int m_iROUNDS;

    char m_chain0[MAX_BLOCK_SIZE];
    char m_chain[MAX_BLOCK_SIZE];

    int tk[MAX_KC];
    int a[MAX_BC];
    int t[MAX_BC];
};

#endif

