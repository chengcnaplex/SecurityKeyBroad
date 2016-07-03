#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include "cap_des2.h"

//typedef unsigned char U08;
///*  8-bit unsigned integer                          */
typedef signed char S08;
/*  8-bit   signed integer                          */
typedef unsigned short U16;
/* 16-bit unsigned integer                          */
typedef signed short S16;
/* 16-bit   signed integer                          */
typedef unsigned int U32;
/* 32-bit unsigned integer                          */
typedef signed int S32;
/* 32-bit   signed integer                          */
typedef float FP32;
/* 32-bit floating point                            */
typedef double FP64;           /* 64-bit floating point       					    */

#define READNUM 8

/*
***************************************************************************************************
                    32位，16位，8位互换结构联合体

  注意： (1) 51里定义INT_32BIT_08BIT...时，不能定义为extern变量    而ARM里可以这样定义
         (2) 51里定义结构体高位在前，而ARM里则底位在前(小端存储模式)

         将一个数进行合并与斥开非常高效的方法
  例：
         INT_32BIT_08BIT a;
         a.Int32Bit  =  0x12345678;	  同样   a.Int08Bit.HH = 0x87;
  则：   a.Int08Bit.HH -> 0x12;				 a.Int08Bit.HL = 0x65;
         a.Int08Bit.HL -> 0x34;			  	 a.Int08Bit.H  = 0x43;
         a.Int08Bit.H  -> 0x56;				 a.Int08Bit.L  = 0x21;
		 a.Int08Bit.L  -> 0x78;		  则：   a.Int32Bit   -> 0x87654321;

***************************************************************************************************
*/

typedef struct                   // <--------|
{                                //          |
    U08 LL;                  //          |
    U08 LH;                 //          |
    U08 HL;                  //          |
    U08 HH;                  //          |32BIT与
} INT_08BIT_4;                   //          |
//          |8BIT转换
typedef union                    //          |
{                                 //          |
    U32 Int32Bit;              //          |
    INT_08BIT_4 Int08Bit;       //          |
} _INT_32_08;                 // <--------|
//========>> 32位<-->8位

typedef struct                   // <--------|
{                                //          |
    U16 L;                  //          |
    U16 H;                  //          |32BIT与
} INT_16BIT_2;                   //          |
//          |16BIT转换
typedef union                    //          |
{                                 //          |
    U32 Int32Bit;              //          |
    INT_16BIT_2 Int16Bit;       //          |
} INT_32_16;                     // <--------|
//========>> 32位<-->16位

typedef struct                   // <--------|
{                                //          |
    U08 L;                  //          |
    U08 H;                  //          |16BIT与
} INT_08BIT_2;                   //          |
//          |8BIT转换
typedef union                    //          |
{                                 //          |
    U16 Int16Bit;              //          |
    INT_08BIT_2 Int08Bit;       //          |
} INT_16_08;                     // <--------|

//========>> 16位<-->8位

#define IP_TABLE    0
#define IP_1_TABLE    1
#define E_TABLE        2
#define P_TABLE        3
#define PC1_TABLE    4
#define PC2_TABLE    5

typedef struct {
    unsigned char L[32];
    unsigned char R[32];
} LRStruct;

typedef struct {
    unsigned char C[28];
    unsigned char D[28];
} CDStruct;

typedef struct {
    unsigned char b0 :    1;
    unsigned char b1 :    1;
    unsigned char b2 :    1;
    unsigned char b3 :    1;
    unsigned char b4 :    1;
    unsigned char b5 :    1;
    unsigned char b6 :    1;
    unsigned char b7 :    1;
} BYTEStruct;

U08 const g_nOut[6] = {64, 64, 48, 32, 56, 48};

U08 const TABLE_shift[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

U08 const TABLE_ip[64] = {
        57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7,
        56, 48, 40, 32, 24, 16, 8, 0, 58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6};
U08 const TABLE_ip_1[64] = {
        39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25, 32, 0, 40, 8, 48, 16, 56, 24};
U08 const TABLE_p[32] = {
        15, 6, 19, 20, 28, 11, 27, 16, 0, 14, 22, 25, 4, 17, 30, 9,
        1, 7, 23, 13, 31, 26, 2, 8, 18, 12, 29, 5, 21, 10, 3, 24};
U08 const TABLE_pc_1[56] = {
        56, 48, 40, 32, 24, 16, 8, 0, 57, 49, 41, 33, 25, 17, 9, 1,
        58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 62, 54, 46, 38,
        30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 60, 52, 44, 36,
        28, 20, 12, 4, 27, 19, 11, 3};
U08 const TABLE_ePro[48] = {
        31, 4, 0, 1, 2, 3, 3, 8, 4, 5, 6, 7, 7, 12, 8, 9,
        10, 11, 11, 16, 12, 13, 14, 15, 15, 20, 16, 17, 18, 19, 19, 24,
        20, 21, 22, 23, 23, 28, 24, 25, 26, 27, 27, 0, 28, 29, 30, 31};
U08 const TABLE_pc_2Pro[48] = {
        13, 4, 16, 10, 23, 0, 2, 9, 27, 14, 5, 20, 22, 7, 18, 11,
        3, 25, 15, 1, 6, 26, 19, 12, 40, 54, 51, 30, 36, 46, 29, 47,
        39, 50, 44, 32, 43, 52, 48, 38, 55, 33, 45, 31, 41, 49, 35, 28};
U08 const TABLE_s[8][64] =
        {
                {14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7,  0,  15, 7,  4,  14, 2,  13, 1,  10, 6, 12, 11, 9,  5,  3,  8, 4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0,  15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0, 6,  13},
                {15, 1,  8,  14, 6,  11, 3,  4,  9, 7,  2,  13, 12, 0,  5,  10, 3,  13, 4,  7, 15, 2, 8,  14, 12, 0, 1,  10, 6, 9, 11, 5, 0, 14, 7,  11, 10, 4, 13, 1,  5,  8,  12, 6, 9, 3,  2, 15, 13, 8,  10, 1, 3, 15, 4, 2, 11, 6,  7, 12, 0,  5, 14, 9},
                {10, 0,  9,  14, 6,  3,  15, 5,  1, 13, 12, 7,  11, 4,  2,  8,  13, 7,  0,  9, 3,  4, 6,  10, 2,  8, 5,  14, 12, 11, 15, 1, 13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5, 10, 14, 7,  1,  10, 13, 0, 6, 9,  8, 7, 4,  15, 14, 3,  11, 5, 2,  12},
                {7,  13, 14, 3,  0,  6,  9,  10, 1, 2,  8,  5,  11, 12, 4,  15, 13, 8,  11, 5, 6,  15, 0,  3,  4,  7, 2,  12, 1,  10, 14, 9, 10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5, 2,  8,  4,  3,  15, 0,  6, 10, 1,  13, 8, 9,  4,  5,  11, 12, 7, 2,  14},
                {2,  12, 4,  1,  7,  10, 11, 6,  8, 5,  3,  15, 13, 0,  14, 9,  14, 11, 2,  12, 4,  7,  13, 1,  5,  0, 15, 10, 3,  9,  8,  6, 4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6, 3,  0,  14, 11, 8,  12, 7, 1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3},
                {12, 1,  10, 15, 9,  2,  6,  8,  0, 13, 3,  4,  14, 7,  5,  11, 10, 15, 4,  2,  7,  12, 9,  5,  6,  1, 13, 14, 0,  11, 3,  8, 9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1, 13, 11, 6,  4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0, 8,  13},
                {4,  11, 2,  14, 15, 0,  8,  13, 3, 12, 9,  7,  5,  10, 6,  1,  13, 0,  11, 7,  4,  9,  1,  10, 14, 3, 5,  12, 2,  15, 8,  6, 1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0, 5,  9,  2,  6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2, 3,  12},
                {13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7,  1,  15, 13, 8,  10, 3,  7,  4,  12, 5, 6,  11, 0,  14, 9,  2, 7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8,  2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5, 6,  11}
        };

void BitToByte(unsigned char *src, unsigned char *dst, int n);

void ByteToBit(unsigned char *src, unsigned char *dst, int n);

void DesAlgo(unsigned char *key, unsigned char *src, unsigned char *dst, unsigned char bEncrypt);

void Shift(CDStruct *pCD, int iCircle, unsigned char bEncrypt);

void RShift(unsigned char *buf28, int nShift);

void LShift(unsigned char *buf28, int nShift);

void fFunction(unsigned char *R, unsigned char *K, unsigned char *result);

void S_Change(unsigned char *src, unsigned char *result);

void SelectExchangeFromTable(int nTable, unsigned char *src, unsigned char *dst);


void BitToByte(unsigned char *src, unsigned char *dst, int n) {
    int i;
    BYTEStruct *BS;
    for (i = 0; i < n; i++) {
        BS = (BYTEStruct *) (&src[i]);
        dst[i * 8 + 7] = BS->b0;
        dst[i * 8 + 6] = BS->b1;
        dst[i * 8 + 5] = BS->b2;
        dst[i * 8 + 4] = BS->b3;
        dst[i * 8 + 3] = BS->b4;
        dst[i * 8 + 2] = BS->b5;
        dst[i * 8 + 1] = BS->b6;
        dst[i * 8 + 0] = BS->b7;
    }
}

void ByteToBit(unsigned char *src, unsigned char *dst, int n) {
    int i;
    BYTEStruct *BS;
    for (i = 0; i < n; i++) {
        BS = (BYTEStruct *) (&dst[i]);
        BS->b0 = src[i * 8 + 7];
        BS->b1 = src[i * 8 + 6];
        BS->b2 = src[i * 8 + 5];
        BS->b3 = src[i * 8 + 4];
        BS->b4 = src[i * 8 + 3];
        BS->b5 = src[i * 8 + 2];
        BS->b6 = src[i * 8 + 1];
        BS->b7 = src[i * 8 + 0];
    }
}

int BitXor(unsigned char *Data1, unsigned char *Data2, int Len, unsigned char *Dest) {
    int i;

    for (i = 0; i < Len; i++) Dest[i] = Data1[i] ^ Data2[i];
    return 0;
}

void MyCopy(unsigned char *out, unsigned char *in, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        out[i] = in[i];
    }
}

void DesAlgo(unsigned char *key, unsigned char *src, unsigned char *dst, unsigned char bEncrypt) {
    int i, j, k;
    unsigned char input[64];
    unsigned char output[64];
    unsigned char KEY[64];
    unsigned char tempA[7];
    unsigned char tempB[64];
    unsigned char tempC[8];
    unsigned char tempD[32];
    unsigned char K[48];
    unsigned char fResult[32];

    LRStruct LR;
    CDStruct CD;

    tempB[0] = tempB[0];

    BitToByte(src, input, 8);
    SelectExchangeFromTable(IP_TABLE, input, (unsigned char *) &LR);

    BitToByte(key, KEY, 8);
    SelectExchangeFromTable(PC1_TABLE, KEY, (unsigned char *) &CD);

    for (i = 0; i < 16; i++) {
        Shift(&CD, i, bEncrypt);
        ByteToBit((unsigned char *) &CD, tempA, 7);
        SelectExchangeFromTable(PC2_TABLE, (unsigned char *) &CD, K);
        for (k = 0; k < 8; k++) {
            tempB[k * 8 + 0] = 0;
            tempB[k * 8 + 1] = 0;
            for (j = 0; j < 6; j++) tempB[k * 8 + (j + 2)] = K[k * 6 + j];
        }
        ByteToBit(tempA, tempC, 8);
        fFunction(LR.R, K, fResult);
        BitXor(LR.L, fResult, 32, fResult);

//		memcpy(LR.L, LR.R, sizeof(unsigned char)*32);
//		memcpy(LR.R, fResult, sizeof(unsigned char)*32);

        MyCopy(LR.L, LR.R, sizeof(unsigned char) * 32);
        MyCopy(LR.R, fResult, sizeof(unsigned char) * 32);
    }

//	memcpy(tempD, LR.L,sizeof(unsigned char)*32);
//	memcpy(LR.L, LR.R,sizeof(unsigned char)*32);
//	memcpy(LR.R, tempD,sizeof(unsigned char)*32);

    MyCopy(tempD, LR.L, sizeof(unsigned char) * 32);
    MyCopy(LR.L, LR.R, sizeof(unsigned char) * 32);
    MyCopy(LR.R, tempD, sizeof(unsigned char) * 32);

    SelectExchangeFromTable(IP_1_TABLE, (unsigned char *) &LR, output);
    ByteToBit(output, dst, 8);
}


void SelectExchangeFromTable(int nTable, unsigned char *src, unsigned char *dst) {
    U32 i, nOut;

    const U08 *table[6] = {TABLE_ip, TABLE_ip_1, TABLE_ePro, TABLE_p, TABLE_pc_1, TABLE_pc_2Pro};
    const U08 *pTable;

    nOut = g_nOut[nTable];
    pTable = table[nTable];

    for (i = 0; i < nOut; i++) { dst[i] = src[pTable[i]]; }
}

void Shift(CDStruct *pCD, int iCircle, unsigned char bEncrypt) {

    if (0 == bEncrypt) {
        if (iCircle != 0) {
            RShift(pCD->C, TABLE_shift[iCircle]);
            RShift(pCD->D, TABLE_shift[iCircle]);
        }
    }
    else {
        LShift(pCD->C, TABLE_shift[iCircle]);
        LShift(pCD->D, TABLE_shift[iCircle]);
    }
}

void RShift(unsigned char *buf28, int nShift) {
    int i;
    unsigned char temp[2];

    for (i = 0; i < nShift; i++) temp[(nShift - 1) - i] = buf28[27 - i];

    for (i = 27; i >= nShift; i--) buf28[i] = buf28[i - nShift];

    for (i = 0; i < nShift; i++) buf28[i] = temp[i];

}

void LShift(unsigned char *buf28, int nShift) {
    int i;
    unsigned char temp[2];

    for (i = 0; i < nShift; i++) temp[i] = buf28[i];

    for (i = nShift; i < 28; i++) buf28[i - nShift] = buf28[i];

    for (i = 0; i < nShift; i++) buf28[27 - (nShift - 1) + i] = temp[i];

}

void fFunction(unsigned char *R, unsigned char *K, unsigned char *result) {
    unsigned char temp[48];
    unsigned char SResult[32];

    SelectExchangeFromTable(E_TABLE, R, temp);

    BitXor(K, temp, 48, temp);

    S_Change(temp, SResult);

    SelectExchangeFromTable(P_TABLE, SResult, result);
}

void S_Change(unsigned char *src, unsigned char *result) {
    unsigned char sTemp;
    unsigned char nPos;
    int i;
    BYTEStruct *rBSPos;

    for (i = 0; i < 8; i++) {
        rBSPos = (BYTEStruct *) (&nPos);
        nPos = 0;

        rBSPos->b0 = src[i * 6 + 5];
        rBSPos->b1 = src[i * 6 + 4];
        rBSPos->b2 = src[i * 6 + 3];
        rBSPos->b3 = src[i * 6 + 2];
        rBSPos->b4 = src[i * 6 + 1];
        rBSPos->b5 = src[i * 6 + 0];

        if (i % 2 == 0) sTemp = TABLE_s[i][nPos] * 16;
        else {
            sTemp += TABLE_s[i][nPos];
            BitToByte(&sTemp, &result[i / 2 * 8], 1);
        }
    }
}

/*
**************************************************************************************************************
*
* 函数原型：void CurCalc_DES_Encrypt( U08 *inkey, U08 *indata, U08 *outdata )
*
* 函数功能：DES加密
*
* 函数输入：inkey		8字节密码
*			indata		8字节需要加密的数据
*
* 函数输出：outdata		8字节加密结果输出
*
* 函数返回：无
*
**************************************************************************************************************
*/
void CurCalc_DES_Encrypt(U08 *inkey, U08 *indata, U08 *outdata) {
    DesAlgo(inkey, indata, outdata, 1);
}

/*
**************************************************************************************************************
*
* 函数原型：void CurCalc_DES_Decrypt( U08 *inkey, U08 *indata, U08 *outdata )
*
* 函数功能：DES解密
*
* 函数输入：inkey		8字节密码
*			indata		8字节需要解密的数据
*
* 函数输出：outdata		8字节解密结果输出
*
* 函数返回：无
*
**************************************************************************************************************
*/
void CurCalc_DES_Decrypt(U08 *inkey, U08 *indata, U08 *outdata) {
    DesAlgo(inkey, indata, outdata, 0);
}

//int main(void) {
//    // FILE *fp = NULL;
//    char outdata[255] = {0};
//    char putdata[255] = {0};
//    int i = 0;
//    // int j = 0;
//    // fp = fopen("./test.txt", "w+");
//    // CurCalc_DES_Encrypt
//    CurCalc_DES_Encrypt("12345678", "qwertyuioasdfghjkl", outdata);
//    for (i = 0; i < strlen(outdata); i++) {
//        //ciphertext[i]+=1;
//        printf("%02x\n ", outdata[i]);
//
//    }

    //--------------------------------解密-----------------------------------------------
//    int ret,j;
//    char buf[1024];
//    int fd;
//    fd = open("./a.txt",O_RDONLY);//读取密文
//    if(fd<0){
//        printf("open errer\n");
//    }
//    ret = 1;
//    while(ret < READNUM){
//        memset(buf,NULL,1024);
//
////        bzero(buf,1024);
//        ret=read(fd,buf,8);
//        if(ret<=0){
//            break;
//        }
//        for(j=0;j<strlen(buf);j++)
//        {
//            printf("%d ",buf[j]);
//        }
//        printf("\n");
//        CurCalc_DES_Decrypt("chanct-gms",buf,putdata);
//        printf("%s\n",putdata);
//    }
//    printf("\n");
//
//    close(fd);



    // CurCalc_DES_Decrypt("chanct-gms",outdata,putdata);

    //printf("%s\n",putdata);
    //fwrite(outdata, 10, 1, fp);
    //fclose(fp);
//    return 0;
//}