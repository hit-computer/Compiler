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
    int token_id;//别识别出来的词语的id号
    int value;//词语的有效值
};

struct varList
{
    char varList[100][128];//允许有100个变量名
    int type[100];
    int num[100];//变量包含的数据个数
    int addr[100];//变量的地址（偏移值）
    int end;
};//这里把函数名与变量名等同对待

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
    int message[10];//传递信息，0位运算类型，1位第一操作数，2位第二个操作数,3目的,4类型1(0isN,1isV,2isR),5类型2,6跳转位置
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

extern char buffer[2][BUFFERSIZE];//缓冲区
extern int numOfBuffer;
extern int lexemeBegin;//开始指针
extern int forward;//结束指针,指向正要读入的字符
extern int N;//11个关键字
extern ifstream inFile;
extern fstream outFile;
extern varList varlist;
extern stack ST;
