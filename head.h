#ifndef HEAD_H
#define HEAD_H
#include <iostream>
using namespace std;
// TODO 实现x^nfx的函数
void x_de_n_fang_cheng_fx(int xfx[4], int a[4]) //* xfx是结果，a是上一步的结果
{
    //! 注意要取模
    //! 既约多项式是 x^4 + x + 1
    //* 保存四次乘法的系数
    if (a[0] == 0)
    {
        for (int i = 0; i < 3; i++)
            xfx[i] = a[i + 1];
    }
    else
    {
        //! 如果乘数首项不为1就需要将 b1x^2+b0x 与 x+1 进行异或
        xfx[1] = a[2];
        xfx[2] = a[3] == 1 ? 0 : 1;
        xfx[3] = 1;
    }
}
// TODO 乘法
int* chengfa(int a[4], int b[4])
{
    //* 储存结果的系数
    int* result = new int[4];
    for (int i = 0; i < 4; i++)
        result[i] = 0;

    //* 记录下x^nfx
    int xfx[4] = { 0 };
    x_de_n_fang_cheng_fx(xfx, a);
    int x2fx[4] = { 0 };
    x_de_n_fang_cheng_fx(x2fx, xfx);
    int x3fx[4] = { 0 };
    x_de_n_fang_cheng_fx(x3fx, x2fx);

    //* 现在需要根据多项式a和b开始异或
    if (b[0] == 1)
        for (int i = 0; i < 4; i++)
            result[i] ^= x3fx[i];
    if (b[1] == 1)
        for (int i = 0; i < 4; i++)
            result[i] ^= x2fx[i];
    if (b[2] == 1)
        for (int i = 0; i < 4; i++)
            result[i] ^= xfx[i];
    if (b[3] == 1)
        for (int i = 0; i < 4; i++)
            result[i] ^= a[i];
    return result;
}
const int s[4][4] = {
    {9, 4, 10, 11},
    {13, 1, 8, 5},
    {6, 2, 0, 3},
    {12, 14, 15, 7} };

const int tihuanwei[16][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 1},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 1, 0, 0},
    {0, 1, 0, 1},
    {0, 1, 1, 0},
    {0, 1, 1, 1},
    {1, 0, 0, 0},
    {1, 0, 0, 1},
    {1, 0, 1, 0},
    {1, 0, 1, 1},
    {1, 1, 0, 0},
    {1, 1, 0, 1},
    {1, 1, 1, 0},
    {1, 1, 1, 1} };
int rcon1[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
int rcon2[8] = { 0, 0, 1, 1, 0, 0, 0, 0 };
int* yihuo8(int* a, int* b)//8位的异或
{
    int* t = new int[8];
    for (int i = 0; i < 8; i++)
        t[i] = a[i] ^ b[i];
    return t;
}

int* yihuo4(int* a, int* b)//4位的异或
{
    int* t = new int[4];
    for (int i = 0; i < 4; i++)
        t[i] = a[i] ^ b[i];
    return t;
}

void s_he_tihuan(int* temp) //使用s盒替换的函数，8位换
{
    int t1 = 2 * temp[0] + temp[1];
    int t2 = 2 * temp[2] + temp[3];
    int t3 = 2 * temp[4] + temp[5];
    int t4 = 2 * temp[6] + temp[7];
    int tihuan1 = s[t1][t2]; //记录替换后的数字
    int tihuan2 = s[t3][t4];
    //* 四位四位进行替换
    for (int i = 0; i < 4; i++)
        temp[i] = tihuanwei[tihuan1][i];
    for (int i = 0; i < 4; i++)
        temp[i + 4] = tihuanwei[tihuan2][i];
}

void zuoyi(int** temp) //循环左移
{
    //! 掉大坑！！！！注意半字节排列的方式，这里应该是第一字节的右半部分和第二字节的右半部分进行替换
    for (int i = 4;i < 8;i++)
    {
        int t = temp[0][i];
        temp[0][i] = temp[1][i];
        temp[1][i] = t;
    }
}
int* g(int* temp, int* rcon) // temp是一个八位的数组,rcon是轮常数
{
    //! 注意这个temp是密钥，不能改动，要复制一个新的进行计算
    int* t = new int[8];
    for (int i = 0; i < 8; i++)
        t[i] = temp[i];
    //* 循环左移
    for (int i = 0; i < 4; i++)
    {
        int tt = t[i + 4];
        t[i + 4] = t[i];
        t[i] = tt;
    }

    //* 进行s盒替换
    s_he_tihuan(t);

    //* 进行轮常数异或
    return yihuo8(t, rcon);
}

void liehunxiao(int** mingwen)
{
    int si_de2jinzhi[4] = { 0, 1, 0, 0 };
    int* m00 = new int[4];
    int* m10 = new int[4];
    int* m01 = new int[4];
    int* m11 = new int[4];
    for (int i = 0; i < 4; i++)
    {
        m00[i] = mingwen[0][i];
        m10[i] = mingwen[0][i + 4];
        m01[i] = mingwen[1][i];
        m11[i] = mingwen[1][i + 4];
    }
    int* n00 = new int[4];
    int* n10 = new int[4];
    int* n01 = new int[4];
    int* n11 = new int[4];
    n00 = yihuo4(m00, chengfa(si_de2jinzhi, m10));//乘法结果是1011
    n10 = yihuo4(chengfa(si_de2jinzhi, m00), m10);//0101
    n01 = yihuo4(m01, chengfa(si_de2jinzhi, m11));//0100
    n11 = yihuo4(chengfa(si_de2jinzhi, m01), m11);//0010
    for (int i = 0; i < 4; i++)
    {
        mingwen[0][i] = n00[i];
        mingwen[0][i + 4] = n10[i];
        mingwen[1][i] = n01[i];
        mingwen[1][i + 4] = n11[i];
    }
}

void lunmiyaojia(int** mingwen, int** key)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 8; j++)
            mingwen[i][j] ^= key[i][j];
}

//用于输出
void shuchu(int** a)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 8; j++)
            cout << a[i][j] << ' ';
    cout << endl;
}


// 解密函数
void decrypt(int** ciphertext, int** key1, int** key2, int** originalKey)
{
    // 第二轮解密
    // 逆轮密钥加
    lunmiyaojia(ciphertext, key2);
    // 逆半字节代替
    for (int i = 0; i < 2; i++)
    {
        int t1 = ciphertext[i][0];
        int t2 = ciphertext[i][1];
        int t3 = ciphertext[i][2];
        int t4 = ciphertext[i][3];
        int t5 = ciphertext[i][4];
        int t6 = ciphertext[i][5];
        int t7 = ciphertext[i][6];
        int t8 = ciphertext[i][7];
        for (int j = 0; j < 4; j++)
        {
            ciphertext[i][j] = t5 + 2 * t6 + 4 * t7 + 8 * t8;
            ciphertext[i][j + 4] = t1 + 2 * t2 + 4 * t3 + 8 * t4;
        }
    }
    // 逆行移位
    for (int i = 4; i < 8; i++)
    {
        int t = ciphertext[0][i];
        ciphertext[0][i] = ciphertext[1][i];
        ciphertext[1][i] = t;
    }

    // 第一轮解密
    // 逆轮密钥加
    lunmiyaojia(ciphertext, key1);
    // 逆半字节代替
    for (int i = 0; i < 2; i++)
    {
        int t1 = ciphertext[i][0];
        int t2 = ciphertext[i][1];
        int t3 = ciphertext[i][2];
        int t4 = ciphertext[i][3];
        int t5 = ciphertext[i][4];
        int t6 = ciphertext[i][5];
        int t7 = ciphertext[i][6];
        int t8 = ciphertext[i][7];
        for (int j = 0; j < 4; j++)
        {
            ciphertext[i][j] = t5 + 2 * t6 + 4 * t7 + 8 * t8;
            ciphertext[i][j + 4] = t1 + 2 * t2 + 4 * t3 + 8 * t4;
        }
    }
    // 逆行移位
    for (int i = 4; i < 8; i++)
    {
        int t = ciphertext[0][i];
        ciphertext[0][i] = ciphertext[1][i];
        ciphertext[1][i] = t;
    }
    // 逆列混淆
    int inverseSiDe2JinZhi[4] = { 1, 0, 1, 1 };
    int* m00 = new int[4];
    int* m10 = new int[4];
    int* m01 = new int[4];
    int* m11 = new int[4];
    for (int i = 0; i < 4; i++)
    {
        m00[i] = ciphertext[0][i];
        m10[i] = ciphertext[0][i + 4];
        m01[i] = ciphertext[1][i];
        m11[i] = ciphertext[1][i + 4];
    }
    int* n00 = new int[4];
    int* n10 = new int[4];
    int* n01 = new int[4];
    int* n11 = new int[4];
    n00 = yihuo4(m00, chengfa(inverseSiDe2JinZhi, m10));
    n10 = yihuo4(chengfa(inverseSiDe2JinZhi, m00), m10);
    n01 = yihuo4(m01, chengfa(inverseSiDe2JinZhi, m11));
    n11 = yihuo4(chengfa(inverseSiDe2JinZhi, m01), m11);
    for (int i = 0; i < 4; i++)
    {
        ciphertext[0][i] = n00[i];
        ciphertext[0][i + 4] = n10[i];
        ciphertext[1][i] = n01[i];
        ciphertext[1][i + 4] = n11[i];
    }
    // 逆轮密钥加
    lunmiyaojia(ciphertext, originalKey);
    delete[] m00;
    delete[] m10;
    delete[] m01;
    delete[] m11;
    delete[] n00;
    delete[] n10;
    delete[] n01;
    delete[] n11;
}
#endif // HEAD_H
