#include <fstream>
#include <iostream>
using namespace std;
#include<string.h>
#include<cctype>
#include <math.h>
#define hash_key 11
#define var 12
#define digit 13
#define lr_brac 14
#define rr_brac 15
#define le_add 16
#define le_sub 17
#define le_div 18
#define le_mul 19
#define equal 20
#define no_less 21
#define less 22
#define biger 23
#define no_biger 24
#define semicolon 25
#define star 26
#define no_equal 27
#define assign 28
#define comma 29
#define ls_brac 30
#define rs_brac 31
#define q_mark 32
#define p_mark 33
#define log_AND 34
#define log_OR 35
#define addone 36
#define subone 37
#define lb_brac 38
#define rb_brac 39
#define le_printf 40
#define le_scanf 41
#define le_string 42
#define le_main 43
#define le_ad 44
#define end_eof 45

#define BUFFERSIZE 1024

extern char *key_word[11];
struct lexical
{
    int token_id;//��ʶ������Ĵ����id��
    int value;//�������Чֵ
};

struct varList
{
    char varList[100][128];//������100��������
    int type[100];
    int num[100];//�������������ݸ���
    int addr[100];//�����ĵ�ַ��ƫ��ֵ��
    int end;
};//����Ѻ��������������ͬ�Դ�

struct Entry
{
    char at;
    int gt;
};

struct Grammar
{
    int pro;
    int num;
};
struct Attr
{
    int key;
    char str[200];
    int value;
    int array[100];
    int message[10];//������Ϣ��0λ�������ͣ�1λ��һ��������2λ�ڶ���������,3Ŀ��,4����1(0isN,1isV,2isR),5����2,6��תλ��
    bool isT;
};
struct stack
{
    int state[9000];
    Attr sym[9000];
    int num1;
    int num2;
};

lexical token_scan();
void getToBuffer(char buffer[]);
char nextChar();
void retract();
void copytoken(char str[]);
int isKeyWord(char str[]);
int toINT(char str[]);
int addVarList(char str[]);
void error_handle();
void passHead();
void freeBuffer();
void display(char str[],int id,int varid);
void Gram();
void emptyS();
void push1(int key);
void push2(Attr key);
int pop1();
Attr pop2();
int look1();
Attr look2();

extern char buffer[2][BUFFERSIZE];//������
extern int numOfBuffer;
extern int lexemeBegin;//��ʼָ��
extern int forward;//����ָ��,ָ����Ҫ������ַ�
extern int N;//11���ؼ���
extern ifstream inFile;
extern fstream outFile;
extern varList varlist;
extern stack ST;
