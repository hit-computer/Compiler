#include "lexical.h"

char buffer[2][BUFFERSIZE];//缓冲区
int numOfBuffer;
int lexemeBegin = 0;//开始指针
int forward = 0;//结束指针,指向正要读入的字符
int N = 11;//11个关键字
ifstream inFile;
fstream outFile;
varList varlist;
char *key_word[] = {"char","do","double","else",
"float","for","int","if","return",
"void","while"};
stack ST;
lexical token_scan()
{
    char c;
    char token[128];
    int id;
    lexical lex;
    while(1)
    {
        c = nextChar();
        if(c == '\0')
        {
            lex.token_id = end_eof;
            return lex;
        }
        while(c == ' '||c == 10||c == 13||c == 9)
        {
            c = nextChar();
            lexemeBegin++;
        }
        if(c == '/' && buffer[numOfBuffer][forward] == '*')
        {
            nextChar();
            c = nextChar();
            while(c != '*' || buffer[numOfBuffer][forward] != '/')
            {
                c = nextChar();
            }
            c = nextChar();
            lexemeBegin = forward;
            continue;
        }
        if(isalpha(c) || c == '_')
        {
            c = nextChar();
            while(isalnum(c) || c == '_')
                c = nextChar();
            retract();
            copytoken(token);
            //cout<<"token :  "<<token<<endl;
            //cout<<lexemeBegin<<"  "<<forward<<endl;
            if((id = isKeyWord(token)) != N)
            {
                lex.token_id = id;
                lex.value = id;
                //cout<<"id:  "<<id<<endl;
                lexemeBegin = forward;
                return lex;
            }
            else if(!strcmp(token,"printf"))
            {
                lex.token_id = le_printf;
                lex.value = le_printf;
                lexemeBegin = forward;
                return lex;
            }
            else if(!strcmp(token,"scanf"))
            {
                lex.token_id = le_scanf;
                lex.value = le_scanf;
                lexemeBegin = forward;
                return lex;
            }
            else if(!strcmp(token,"main"))
            {
                lex.token_id = le_main;
                lex.value = le_main;
                lexemeBegin = forward;
                return lex;
            }
            else
            {
                lex.token_id = var;
                lex.value = addVarList(token);
                lexemeBegin = forward;
                return lex;
            }
        }
        else if(isdigit(c))
        {
            c = nextChar();
            while(isdigit(c))
                c = nextChar();
            retract();
            copytoken(token);
            lex.token_id = digit;
            lex.value = toINT(token);
            lexemeBegin = forward;
            return lex;
        }
        else
        {
            switch(c)
            {
                case '!':
                    c = nextChar();
                    if(c == '=')
                    {
                        //retract();
                        lex.token_id = no_equal;
                        lex.value = no_equal;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else
                    {
                        error_handle();
                    }
                    break;
                case '=':
                    c = nextChar();
                    if(c != '=')
                    {
                        retract();
                        lex.token_id = assign;
                        lex.value = assign;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else
                    {
                        lex.token_id = equal;
                        lex.value = equal;
                        lexemeBegin = forward;
                        return lex;
                    }
                    break;
                case '*':
                    lex.token_id = le_mul;
                    lex.value = le_mul;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case '<':
                    c = nextChar();
                    if(c == '=')
                    {
                        lex.token_id = no_biger;
                        lex.value = no_biger;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else
                    {
                        retract();
                        lex.token_id = less;
                        lex.value = less;
                        lexemeBegin = forward;
                        return lex;
                    }
                    break;
                case '>':
                    c  = nextChar();
                    if(c == '=')
                    {
                        lex.token_id = no_less;
                        lex.value = no_less;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else
                    {
                        retract();
                        lex.token_id = biger;
                        lex.value = biger;
                        lexemeBegin = forward;
                        return lex;
                    }
                    break;
                case '|':
                    c = nextChar();
                    if (c == '|')
                    {
                        lex.token_id = log_OR;
                        lex.value = log_OR;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else{
                        error_handle();
                    }
                    break;
                case '&':
                    c = nextChar();
                    if(c=='&')
                    {
                        lex.token_id = log_AND;
                        lex.value = log_AND;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else{
                        lex.token_id = le_ad;
                        lex.value = le_ad;
                        lexemeBegin = forward;
                        return lex;
                    }
                    break;
                case '+':
                    c = nextChar();
                    if(c == '+')
                    {
                        lex.token_id = addone;
                        lex.value = addone;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else{
                        retract();
                        lex.token_id = le_add;
                        lex.value = le_add;
                        lexemeBegin = forward;
                        return lex;
                    }
                    break;
                case '-':
                    c = nextChar();
                    if(c == '-')
                    {
                        lex.token_id = subone;
                        lex.value = subone;
                        lexemeBegin = forward;
                        return lex;
                    }
                    else{
                        retract();
                        lex.token_id = le_sub;
                        lex.value = le_sub;
                        lexemeBegin = forward;
                        return lex;
                    }
                    break;
                case '/':
                    lex.token_id = le_div;
                    lex.value = le_div;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case ',':
                    lex.token_id = comma;
                    lex.value = comma;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case ';':
                    lex.token_id = semicolon;
                    lex.value = semicolon;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case '(':
                    lex.token_id = lr_brac;
                    lex.value = lr_brac;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case ')':
                    lex.token_id = rr_brac;
                    lex.value = rr_brac;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case '[':
                    lex.token_id = ls_brac;
                    lex.value = ls_brac;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case ']':
                    lex.token_id = rs_brac;
                    lex.value = rs_brac;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case '{':
                    lex.token_id = lb_brac;
                    lex.value = lb_brac;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case '}':
                    lex.token_id = rb_brac;
                    lex.value = rb_brac;
                    lexemeBegin = forward;
                    return lex;
                    break;
                case '\"'://""之间的内容在编译后生成的代码中应放在变量申明部分
                    lexemeBegin = forward;
                    c = nextChar();
                    while(c != '\"')
                    {
                        c = nextChar();
                    }
                    retract();
                    copytoken(token);
                    //cout<<"token :  "<<token<<endl;
                    lex.token_id = le_string;
                    lex.value = addVarList(token);
                    nextChar();
                    lexemeBegin = forward;
                    return lex;
                    break;
                default :error_handle();
                    break;
            }
        }
    }
}

char nextChar()
{
    char ch;
    ch = buffer[numOfBuffer][forward];
    if(ch == '\0')
        return '\0';
    forward++;
    freeBuffer();
    //cout<<"ch: "<<ch<<"  "<<(int)ch<<endl;
    return ch;
}
void retract()
{
    forward--;
}
void copytoken(char str[])
{
    int i = 0;
    int b = lexemeBegin;
    int e = forward;
    if(e < b)
    {
        for(;b < BUFFERSIZE;b++)
        {
            int t = (numOfBuffer+1)%2;
            str[i] = buffer[t][b];
            i++;
        }
        for(b = 0;b < e;b++)
        {
            str[i] = buffer[numOfBuffer][b];
            i++;
        }
    }
    else{
        for(;b < e;b++)
        {
            str[i] = buffer[numOfBuffer][b];
            i++;
        }
    }
    str[i] = '\0';
    lexemeBegin = forward;
}
int isKeyWord(char str[])
{
    int i = 0;
    for(i = 0; i < N ;i++)
    {
        if(!strcmp(str,key_word[i]))
            break;
    }
    return i;
}
int toINT(char str[])
{
    int n = 0;
    int value = 0;
    for(;str[n]!='\0';n++)
    ;
    for(int i = 0;i<n;i++)
    {
        value += pow(10,n-1-i)*(str[i] - '0');
    }
    return value;
}
int addVarList(char str[])
{
    int i = 0;
    for(i = 0; i < varlist.end ;i++)
    {
        if(!strcmp(str,varlist.varList[i]))
            break;
    }
    if(i == varlist.end)
    {
        strcpy(varlist.varList[i],str);
        varlist.end++;
    }
    return i;
}
void error_handle()
{
    cout<<"Error!!"<<endl;
    //exit(0);
}
void passHead()
{
    memset(buffer[0],0,sizeof(buffer[0]));
    inFile.read(buffer[0],BUFFERSIZE);
    char ch;
    ch  = nextChar();
    while(ch == '#')
    {
        while(ch != '\n')
        {
            ch = nextChar();
            lexemeBegin++;
        }
        ch = nextChar();
        lexemeBegin++;
    }
    retract();
    lexemeBegin = forward;
}
void freeBuffer()
{
    if(forward >= BUFFERSIZE)
    {
        if(numOfBuffer == 0)
        {
            memset(buffer[1],0,sizeof(buffer[1]));
            inFile.read(buffer[1],BUFFERSIZE);
            numOfBuffer = 1;
            forward = 0;
        }
        else
        {
            memset(buffer[0],0,sizeof(buffer[0]));
            inFile.read(buffer[0],BUFFERSIZE);
            numOfBuffer = 0;
            forward = 0;
        }
    }
}
void display(char str[],int id,int varid)
{
    if(id < N){
        strcpy(str,key_word[id]);
        return ;
    }
    else
    {
        char *name[] = {"hskey","variable","dig","'('","')'","add","sub","div","mul","eq","noless","less","biger","no_biger",
"semicolon","star","no_equal","assign","comma","'['","']'","q_mark","p_mark","log_AND","log_OR","addone","subone","'{'",
"'}'","printf","scanf","string","main"};
        if(id - N == 1)
        {
            int i = 0;
            for(i=0;name[id-N][i]!='\0';i++)
            {
                str[i] = name[id-N][i];
            }
            str[i] = ',';
            str[i+1] = '\"';
            char var1[128];
            strcpy(var1,varlist.varList[varid]);
            int t =0;
            for(t = 0;var1[t]!='\0';t++)
            {
                str[i+2+t] = var1[t];
            }
            str[i+2+t] = '\"';
            str[i+3+t] = '\0';
            //cout<<str<<endl;
        }
        else{
            strcpy(str,name[id - N]);
        }
    }
}
void emptyS()
{
    ST.num1 = 0;
    ST.num2 = 0;
}
void push1(int key)
{
    ST.state[ST.num1] = key;
    ST.num1++;
}
void push2(Attr key)
{
    ST.sym[ST.num2] = key;
    ST.num2++;
}
int pop1()
{
    ST.num1--;
    return ST.state[ST.num1];
}
Attr pop2()
{
    ST.num2--;
    return ST.sym[ST.num2];
}
int look1()
{
    return ST.state[ST.num1-1];
}
Attr look2()
{
    return ST.sym[ST.num2-1];
}
