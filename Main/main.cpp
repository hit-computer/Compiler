#include "lexical.h"

int main()
{
    //freopen("out.txt","w",stdout);
/*
    inFile.open("test.c",ios::in);
    passHead();
    lexical lex;
    lex = token_scan();
    //char ch = nextChar();
    //cout<<lexemeBegin<<"   "<<forward<<endl;
    while(lex.token_id != end_eof)
    {
        char str[140];
        display(str,lex.token_id,lex.value);
        cout<<"("<<lex.token_id<<" , "<<lex.value<<")"<<'\t'<<"("<<str<<" , "<<lex.value<<")"<<endl;
        lex = token_scan();
    }
    for(int t = 0;t < varlist.end;t++)
        cout<<t<<": "<<varlist.varList[t]<<endl;

*/
    //inFile.open("test.c",ios::binary);
    //passHead();
    //cout<<buffer[0];
    string str;
    outFile.open("temp.txt",ios::out);
    Gram();
    outFile<<endl;
    outFile<<".section .data"<<endl;
    int max = 0;
    for(int t = 0;t < varlist.end;t++)
    {
        if(varlist.type[t] == le_string)
        {
            outFile<<"text"<<t<<": .ascii "<<"\""<<varlist.varList[t]<<"\\0"<<"\""<<endl;
        }
        else
        {
            if(varlist.addr[t] > max)
                max = varlist.addr[t];
        }
    }
    outFile.close();
    outFile.open("temp.txt");
    cout<<".section .text"<<endl<<".globl _main"<<endl<<"_main:"<<endl;
    cout<<"pushl %ebp"<<endl<<"movl %esp, %ebp"<<endl;
    cout<<"subl $0x"<<hex<<max<<", %esp"<<endl;
    while(getline(outFile, str))
    {
        cout << str << endl;
    }
    cout<<endl<<endl<<endl;
    for(int t = 0;t < varlist.end;t++)
        cout<<t<<": "<<varlist.varList[t]<<"  "<<dec<<varlist.num[t]<<"  "<<dec<<varlist.addr[t]<<endl;
    return 1;
}
