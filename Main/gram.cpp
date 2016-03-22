#include "lexical.h"

void Gram()
{
    int Base = 47;
    Entry Action[700][46];
    int apos = 0;
    Grammar Gram[120];
    char G[120][100];
    int num_of_G = 0;
    fstream file("Action.txt");
    int i,t,b;
    int jumpN = 0;
    bool isBusy[] = {1,0,0,0};
    char ch;
    while(!file.fail())
    {
        file>>i;
        file>>t;
        file>>ch;
        file>>b;
        //outFile<<i<<"  "<<t<<"  "<<ch<<"  "<<b<<endl;
        Action[i][t].at = ch;
        Action[i][t].gt = b;
    }
    file.close();
    fstream iF("Goto.txt");
    int Goto[700][40];
    while(!iF.fail())
    {
        iF>>i;
        iF>>t;
        iF>>b;
        Goto[i][t]=b;
    }
    iF.close();
    fstream inGram("newg.txt");
    while(!inGram.fail())
    {
        inGram>>i;
        inGram>>t;
        Gram[num_of_G].pro = i;
        Gram[num_of_G].num = t;
        num_of_G++;
    }
    num_of_G -= 1;
    inGram.close();
    int ti = 0;
    fstream iGram("gram.txt");
    while(!iGram.fail())
    {
        iGram.getline(G[ti],sizeof(G[ti]));
        //outFile<<G[ti]<<endl;
        ti++;
    }
    iGram.close();


    inFile.open("test.c",ios::binary);
    passHead();
    lexical lex;
    lex = token_scan();
    int leId,st,sym;
    char at;
    Attr att;
    push1(0);
    att.key = 45;
    push2(att);
    char str[140];
    while(1)
    {
        //outFile<<"----------------------------------------------------"<<endl;
        //display(str,lex.token_id,lex.value);
        //outFile<<"("<<lex.token_id<<" , "<<lex.value<<")"<<'\t'<<"("<<str<<" , "<<lex.value<<")"<<endl;
        //lex = token_scan();
        leId = lex.token_id;
        st = look1();
        at = Action[st][leId].at;
        //outFile<<"action: "<<at<<"  st: "<<st<<"  id: "<<leId<<endl;
        if(at == 'S')
        {
            push1(Action[st][leId].gt);
            att.key = leId;
            att.value = lex.value;
            push2(att);
            if(leId != 45)
                lex = token_scan();
        }
        else if(at == 'R')
        {
            int gid = Action[st][leId].gt;
            int pro = Gram[gid].pro;
            int n = Gram[gid].num;
            //cout<<"产生式: "<<dec<<gid<<"   "<<G[gid]<<endl;
            Attr newattr;
            Attr attr2;
            int num;
            int nt,ni,nb;
            int addr;
            switch(gid)
            {
                case 5://<R>->int
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"");
                    att.value = newattr.value;
                    break;
                case 4://<FH>->dig
                case 57://<MC>->dig
                    strcpy(att.str,"num");
                    pop1();
                    newattr = pop2();
                    att.value = newattr.value;
                    break;
                case 49://<M>-><MC>
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,newattr.str);
                    att.value = newattr.value;
                    for(int i=0;i<10;i++)
                    {
                        att.message[i] = newattr.message[i];
                    }
                    //outFile<<att.value<<endl;
                    break;
                case 31://<A>->[ <M> ]
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    pop1();
                    pop2();
                    att.value = newattr.value;
                    strcpy(att.str,newattr.str);
                    //outFile<<att.value<<endl;
                    break;
                case 37://<CN>->, dig
                    strcpy(att.str,"");
                    pop1();
                    newattr = pop2();
                    pop1();
                    pop2();
                    att.value = 0;
                    att.array[att.value] = newattr.value;
                    //outFile<<att.array[att.value]<<endl;
                    break;
                case 36://<CN>->, dig <CN>
                    strcpy(att.str,"");
                    pop1();
                    newattr = pop2();
                    att.value = newattr.value+1;
                    for(int ai=0;ai<att.value;ai++)
                    {
                        att.array[ai] = newattr.array[ai];
                    }
                    pop1();
                    newattr = pop2();
                    att.array[att.value] = newattr.value;
                    pop1();
                    pop2();
                    /*for(int ai=0;ai<att.value+1;ai++)
                    {
                        outFile<<att.array[ai]<<"   ";
                    }
                    outFile<<endl;*/
                    break;
                case 35://<NS>->dig <CN>
                    strcpy(att.str,"");
                    pop1();
                    newattr = pop2();
                    att.value = newattr.value+1;
                    for(int ai=0;ai<att.value;ai++)
                    {
                        att.array[ai] = newattr.array[ai];
                    }
                    pop1();
                    newattr = pop2();
                    att.array[att.value] = newattr.value;
                    break;
                case 33://<DF>->= { <NS> }
                    strcpy(att.str,"");
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    att.value = newattr.value;
                    att.isT = 1;
                    for(int ai=0;ai<att.value+1;ai++)
                    {
                        att.array[att.value-ai] = newattr.array[ai];
                    }
                    pop1();
                    pop2();
                    pop1();
                    pop2();
                    break;
                case 20://<D>->int var <A> <DF> ;
                    strcpy(att.str,"");
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    att.value = newattr.value;
                    for(int ai=0;ai<att.value+1;ai++)
                    {
                        att.array[ai] = newattr.array[ai];
                    }
                    pop1();
                    newattr = pop2();
                    num = newattr.value;
                    pop1();
                    newattr = pop2();
                    nt = newattr.value;
                    varlist.type[nt] = 6;
                    varlist.num[nt] = num;
                    if(nt > 0)
                        varlist.addr[nt] = varlist.addr[nt-1]+4*num;
                    else
                        varlist.addr[nt] = 4*num;
                    pop1();
                    pop2();
                    att.str[0] = nt;
                    att.isT = 1;
                    //outFile<<varlist.addr[nt]<<endl;
                    break;
                case 9://<BS>-><D>
                    pop1();
                    newattr = pop2();
                    if(!newattr.isT)
                        break;
                    nt = newattr.value;
                    ni = newattr.str[0];
                    addr = varlist.addr[ni];
                    for(int i=0;i<=nt;i++)
                    {
                        outFile<<"movl $0x"<<hex<<newattr.array[i]<<",-0x"<<hex<<addr-4*i<<"(%ebp)"<<endl;
                    }
                    break;
                case 45://<DS>->, var
                    strcpy(att.str,"");
                    pop1();
                    newattr = pop2();
                    att.value = 0;
                    att.array[att.value] = newattr.value;
                    pop1();
                    pop2();
                    break;
                case 41://<DS>->, var <DS>
                    strcpy(att.str,"");
                    pop1();
                    newattr = pop2();
                    att.value = newattr.value+1;
                    for(int ai=0;ai<att.value;ai++)
                    {
                        att.array[ai] = newattr.array[ai];
                    }
                    pop1();
                    newattr = pop2();
                    att.array[att.value] = newattr.value;
                    pop1();
                    pop2();
                    break;
                case 25://<D>->int var <DS> ;
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    att.value = newattr.value;
                    for(int ai=0;ai<att.value+1;ai++)
                    {
                        att.array[att.value-ai+1] = newattr.array[ai];
                    }
                    pop1();
                    newattr = pop2();
                    att.array[0] = newattr.value;
                    num = 1;
                    for(int i=0;i<=att.value+1;i++)
                    {
                        nt = att.array[i];
                        varlist.type[nt] = 6;
                        varlist.num[nt] = num;
                        if(nt > 0)
                            varlist.addr[nt] = varlist.addr[nt-1]+4*num;
                        else
                            varlist.addr[nt] = 4*num;
                    }
                    pop1();
                    pop2();
                    att.isT = 0;
                    break;
                case 32://<DF>->= <M>
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,newattr.str);
                    att.value = newattr.value;
                    pop1();
                    pop2();
                    break;
                case 28://<F>->var <DF> ;
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,newattr.str);
                    att.value = newattr.value;
                    pop1();
                    newattr = pop2();
                    att.message[0] = 0;
                    nt = newattr.value;
                    att.message[3] = varlist.addr[nt];
                    break;
                case 10://<BS>-><F>
                case 66://<FOD>-><F>
                    pop1();
                    newattr = pop2();
                    if(!strcmp(newattr.str,"num"))
                        outFile<<"movl $0x"<<hex<<newattr.value<<",-0x"<<hex<<newattr.message[3]<<"(%ebp)"<<endl;
                    else if(!strcmp(newattr.str,"reg"))
                    {
                        outFile<<"movl ";
                        switch(newattr.value)
                        {
                            case 0:
                            outFile<<"%eax,";
                            break;
                            case 1:
                            outFile<<"%ebx,";
                            break;
                            case 2:
                            outFile<<"%ecx,";
                            break;
                            case 3:
                            outFile<<"%edx,";
                            break;
                        }
                        outFile<<"-0x"<<hex<<newattr.message[3]<<"(%ebp)"<<endl;
                        isBusy[newattr.value] = 0;
                        isBusy[0] = 1;
                    }
                    break;
                case 60://<MC>->var
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"var");
                    nt = newattr.value;
                    att.value = varlist.addr[nt];
                    break;
                case 82://<GK>-><
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"");
                    att.value = newattr.value;
                    break;
                case 74://<T>-><M> <GK> <M>
                    pop1();
                    newattr = pop2();
                    //outFile<<newattr.str<<endl;
                    if(!strcmp(newattr.str,"num"))//<M>是数字
                    {
                        att.message[2] = newattr.value;
                        att.message[5] = 0;
                    }
                    else if(!strcmp(newattr.str,"var")){
                        att.message[2] = varlist.addr[newattr.value];
                        //outFile<<att.message[2]<<endl;
                        att.message[5] = 1;
                    }
                    pop1();
                    newattr = pop2();
                    att.message[0] = newattr.value;
                    pop1();
                    newattr = pop2();
                    if(!strcmp(newattr.str,"num"))//<M>是数字
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 0;
                    }
                    else if(!strcmp(newattr.str,"var")){
                        att.message[1] = newattr.value;
                        att.message[4] = 1;
                    }
                    break;
                case 67://<FOT>-><T> ;
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    switch(newattr.message[0])
                    {
                        case equal:
                            break;
                        case no_less:
                            break;
                        case less:
                            outFile<<"L"<<jumpN<<":";
                            jumpN++;
                            outFile<<"cmpl ";
                            if(newattr.message[5])
                                outFile<<"-0x"<<hex<<newattr.message[2]<<"(%ebp),";
                            else
                                outFile<<"$0x"<<hex<<newattr.message[2]<<",";
                            if(newattr.message[4])
                                outFile<<"-0x"<<hex<<newattr.message[1]<<"(%ebp)";
                            else
                                outFile<<"$0x"<<hex<<newattr.message[1];
                            outFile<<endl;
                            outFile<<"jae "<<"L"<<jumpN<<endl;
                            break;
                        case biger:
                            break;
                        case no_biger:
                            break;
                    }
                    break;
                case 16://<Z>->var ++
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"var");
                    att.value = varlist.addr[newattr.value];
                    att.message[0] = 1;
                    break;
                case 71://<FOF>-><Z>
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,newattr.str);
                    att.message[0] = newattr.message[0];
                    att.message[1] = newattr.value;
                    break;
                case 59://<MC>->var <A>
                    pop1();
                    newattr = pop2();
                    //outFile<<newattr.str<<endl;
                    if(!strcmp(newattr.str,"var"))
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 1;
                    }
                    else if(!strcmp(newattr.str,"reg"))
                    {
                        nt = newattr.value;
                        att.message[1] = nt;
                        att.message[4] = 2;
                    }
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"array");
                    att.value = varlist.addr[newattr.value];
                    //outFile<<att.value<<endl;
                    break;
                case 54://<MS>->* <MC>
                    pop1();
                    newattr = pop2();
                    if(!strcmp(newattr.str,"array") && newattr.message[4]==1)//这是一个以变量为下标的数组
                    {
                        for(ni=0;ni<4;ni++)
                        {
                            if(!isBusy[ni])
                                break;
                        }
                        isBusy[ni] = 1;
                        outFile<<"movl -0x"<<hex<<newattr.message[1]<<"(%ebp),";
                        switch(ni)
                        {
                            case 0:
                            outFile<<"%eax"<<endl;
                            break;
                            case 1:
                            outFile<<"%ebx"<<endl;
                            break;
                            case 2:
                            outFile<<"%ecx"<<endl;
                            break;
                            case 3:
                            outFile<<"%edx"<<endl;
                            break;
                        }
                        addr = newattr.value;
                        outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                        switch(ni)
                        {
                            case 0:
                            outFile<<"%eax";
                            break;
                            case 1:
                            outFile<<"%ebx";
                            break;
                            case 2:
                            outFile<<"%ecx";
                            break;
                            case 3:
                            outFile<<"%edx";
                            break;
                        }
                        for(nt=0;nt<4;nt++)
                        {
                            if(!isBusy[nt])
                                break;
                        }
                        outFile<<",4),";
                        switch(nt)
                        {
                            case 0:
                            outFile<<"%eax"<<endl;
                            break;
                            case 1:
                            outFile<<"%ebx"<<endl;
                            break;
                            case 2:
                            outFile<<"%ecx"<<endl;
                            break;
                            case 3:
                            outFile<<"%edx"<<endl;
                            break;
                        }
                        isBusy[ni] = 0;
                        isBusy[nt] = 1;
                        isBusy[0] = 1;
                        att.message[1] = nt;
                        att.message[4] = 2;
                        att.message[0] = 3;
                    }
                    pop1();
                    pop2();
                    break;
                case 47://<M>-><MC> <MS>
                    pop1();
                    newattr = pop2();
                    if(newattr.message[0]==3)//乘法操作
                    {
                        pop1();
                        attr2 = pop2();
                        if(!strcmp(attr2.str,"array") && attr2.message[4]==1)//这是一个以变量为下标的数组
                        {
                            for(ni=0;ni<4;ni++)
                            {
                                if(!isBusy[ni])
                                    break;
                            }
                            isBusy[ni] = 1;
                            outFile<<"movl -0x"<<hex<<attr2.message[1]<<"(%ebp),";
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax"<<endl;
                                break;
                                case 1:
                                outFile<<"%ebx"<<endl;
                                break;
                                case 2:
                                outFile<<"%ecx"<<endl;
                                break;
                                case 3:
                                outFile<<"%edx"<<endl;
                                break;
                            }
                            addr = attr2.value;
                            outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax";
                                break;
                                case 1:
                                outFile<<"%ebx";
                                break;
                                case 2:
                                outFile<<"%ecx";
                                break;
                                case 3:
                                outFile<<"%edx";
                                break;
                            }
                            outFile<<",4),%eax"<<endl;
                            isBusy[ni] = 0;
                            isBusy[0] = 1;
                        }
                        else if(!strcmp(attr2.str,"var"))
                        {
                            outFile<<"movl -0x"<<hex<<attr2.value<<"(%ebp)"<<endl;
                        }
                        if(newattr.message[4]==2)
                        {
                            outFile<<"imull ";
                            ni = newattr.message[1];
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax,%eax"<<endl;
                                break;
                                case 1:
                                outFile<<"%ebx,%eax"<<endl;
                                break;
                                case 2:
                                outFile<<"%ecx,%eax"<<endl;
                                break;
                                case 3:
                                outFile<<"%edx,%eax"<<endl;
                                break;
                            }
                        }
                        else if(newattr.message[4] == 1)
                        {
                            outFile<<"imull -0x"<<hex<<newattr.message[1]<<"(%ebp),%eax"<<endl;
                        }
                        strcpy(att.str,"reg");
                        att.value = 0;
                    }
                    else if(newattr.message[0]==4)
                    {
                        pop1();
                        attr2 = pop2();
                        if(!strcmp(attr2.str,"array") && attr2.message[4]==1)//这是一个以变量为下标的数组
                        {
                            for(ni=0;ni<4;ni++)
                            {
                                if(!isBusy[ni])
                                    break;
                            }
                            isBusy[ni] = 1;
                            outFile<<"movl -0x"<<hex<<newattr.message[1]<<"(%ebp),";
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax"<<endl;
                                break;
                                case 1:
                                outFile<<"%ebx"<<endl;
                                break;
                                case 2:
                                outFile<<"%ecx"<<endl;
                                break;
                                case 3:
                                outFile<<"%edx"<<endl;
                                break;
                            }
                            addr = newattr.value;
                            outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax";
                                break;
                                case 1:
                                outFile<<"%ebx";
                                break;
                                case 2:
                                outFile<<"%ecx";
                                break;
                                case 3:
                                outFile<<"%edx";
                                break;
                            }
                            outFile<<",4),%eax"<<endl;
                            isBusy[ni] = 0;
                            isBusy[0] = 1;
                        }
                        else if(!strcmp(attr2.str,"var"))
                        {
                            outFile<<"movl -0x"<<hex<<attr2.value<<"(%ebp),%eax"<<endl;
                        }
                        outFile<<"cltd"<<endl;
                        if(newattr.message[4]==2)
                        {
                            outFile<<"idivl ";
                            ni = newattr.message[1];
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax"<<endl;
                                break;
                                case 1:
                                outFile<<"%ebx"<<endl;
                                break;
                                case 2:
                                outFile<<"%ecx"<<endl;
                                break;
                                case 3:
                                outFile<<"%edx"<<endl;
                                break;
                            }
                        }
                        else if(newattr.message[4] == 1)
                        {
                            outFile<<"idivl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<endl;
                        }
                        strcpy(att.str,"reg");
                        att.value = 0;
                    }
                    break;
                case 50://<MF>->+ <M>
                    pop1();
                    newattr = pop2();
                    att.message[0] = 1;
                    //outFile<<newattr.message[4]<<endl;
                    if(!strcmp(newattr.str,"reg"))
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 2;
                    }
                    else if(!strcmp(newattr.str,"num"))
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 0;
                    }
                    else if(!strcmp(newattr.str,"array"))
                    {
                        if(newattr.message[4] == 2)
                        {
                            for(ni=0;ni<4;ni++)
                            {
                                if(!isBusy[ni])
                                    break;
                            }
                            isBusy[ni] = 1;
                            addr = newattr.value;
                            outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                            switch(newattr.message[1])
                            {
                                case 0:
                                outFile<<"%eax";
                                break;
                                case 1:
                                outFile<<"%ebx";
                                break;
                                case 2:
                                outFile<<"%ecx";
                                break;
                                case 3:
                                outFile<<"%edx";
                                break;
                            }
                            outFile<<",4),";
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax"<<endl;
                                break;
                                case 1:
                                outFile<<"%ebx"<<endl;
                                break;
                                case 2:
                                outFile<<"%ecx"<<endl;
                                break;
                                case 3:
                                outFile<<"%edx"<<endl;
                                break;
                            }
                            isBusy[newattr.message[1]] = 0;
                            isBusy[0] = 1;
                            att.message[1] = ni;
                            att.message[4] = 2;
                        }
                    }
                    pop1();
                    pop2();
                    break;
                case 48://<M>-><MC> <MF>
                    pop1();
                    newattr = pop2();
                    if(newattr.message[0]==1)//加法操作
                    {
                        pop1();
                        attr2 = pop2();
                        if(newattr.message[4]==2)
                        {
                            outFile<<"addl ";
                            if(!strcmp(attr2.str,"array") && attr2.message[4]==1)//!!这是一个以变量为下标的数组
                            {
                                for(ni=0;ni<4;ni++)
                                {
                                    if(!isBusy[ni])
                                        break;
                                }
                                isBusy[ni] = 1;
                                outFile<<"movl -0x"<<hex<<newattr.message[1]<<"(%ebp),";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax"<<endl;
                                    break;
                                    case 1:
                                    outFile<<"%ebx"<<endl;
                                    break;
                                    case 2:
                                    outFile<<"%ecx"<<endl;
                                    break;
                                    case 3:
                                    outFile<<"%edx"<<endl;
                                    break;
                                }
                                addr = newattr.value;
                                outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax";
                                    break;
                                    case 1:
                                    outFile<<"%ebx";
                                    break;
                                    case 2:
                                    outFile<<"%ecx";
                                    break;
                                    case 3:
                                    outFile<<"%edx";
                                    break;
                                }
                                outFile<<",4),";
                                isBusy[ni] = 0;
                                isBusy[0] = 1;
                            }
                            else if(!strcmp(attr2.str,"var"))//这是一个变量
                            {
                                outFile<<"-0x"<<hex<<attr2.value<<"(%ebp),";
                            }
                            ni = newattr.message[1];
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax"<<endl;
                                break;
                                case 1:
                                outFile<<"%ebx"<<endl;
                                break;
                                case 2:
                                outFile<<"%ecx"<<endl;
                                break;
                                case 3:
                                outFile<<"%edx"<<endl;
                                break;
                            }
                        }
                        strcpy(att.str,"reg");
                        att.value = newattr.message[1];
                    }
                    else if(newattr.message[0]==2)//减法
                    {
                        pop1();
                        attr2 = pop2();
                        //outFile<<newattr.message[4]<<endl;
                        if(newattr.message[4]==2)
                        {
                            outFile<<"subl ";
                            if(!strcmp(attr2.str,"array") && attr2.message[4]==1)//!!这是一个以变量为下标的数组
                            {
                                for(ni=0;ni<4;ni++)
                                {
                                    if(!isBusy[ni])
                                        break;
                                }
                                isBusy[ni] = 1;
                                outFile<<"movl -0x"<<hex<<newattr.message[1]<<"(%ebp),";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax"<<endl;
                                    break;
                                    case 1:
                                    outFile<<"%ebx"<<endl;
                                    break;
                                    case 2:
                                    outFile<<"%ecx"<<endl;
                                    break;
                                    case 3:
                                    outFile<<"%edx"<<endl;
                                    break;
                                }
                                addr = newattr.value;
                                outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax";
                                    break;
                                    case 1:
                                    outFile<<"%ebx";
                                    break;
                                    case 2:
                                    outFile<<"%ecx";
                                    break;
                                    case 3:
                                    outFile<<"%edx";
                                    break;
                                }
                                outFile<<",4),";
                                isBusy[ni] = 0;
                                isBusy[0] = 1;
                            }
                            else if(!strcmp(attr2.str,"var"))//这是一个变量
                            {
                                outFile<<"-0x"<<hex<<attr2.value<<"(%ebp),";
                            }
                            ni = newattr.message[1];
                            switch(ni)
                            {
                                case 0:
                                outFile<<"%eax"<<endl;
                                break;
                                case 1:
                                outFile<<"%ebx"<<endl;
                                break;
                                case 2:
                                outFile<<"%ecx"<<endl;
                                break;
                                case 3:
                                outFile<<"%edx"<<endl;
                                break;
                            }
                        }
                        else if(newattr.message[4]==0)//第一个是数字
                        {
                            outFile<<"movl ";
                            if(!strcmp(attr2.str,"array") && attr2.message[4]==1)//!!这是一个以变量为下标的数组
                            {
                                for(ni=0;ni<4;ni++)
                                {
                                    if(!isBusy[ni])
                                        break;
                                }
                                isBusy[ni] = 1;
                                outFile<<"movl -0x"<<hex<<newattr.message[1]<<"(%ebp),";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax"<<endl;
                                    break;
                                    case 1:
                                    outFile<<"%ebx"<<endl;
                                    break;
                                    case 2:
                                    outFile<<"%ecx"<<endl;
                                    break;
                                    case 3:
                                    outFile<<"%edx"<<endl;
                                    break;
                                }
                                addr = newattr.value;
                                outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax";
                                    break;
                                    case 1:
                                    outFile<<"%ebx";
                                    break;
                                    case 2:
                                    outFile<<"%ecx";
                                    break;
                                    case 3:
                                    outFile<<"%edx";
                                    break;
                                }
                                outFile<<",4),";
                                isBusy[ni] = 0;
                                isBusy[0] = 1;
                            }
                            else if(!strcmp(attr2.str,"var"))//这是一个变量
                            {
                                outFile<<"-0x"<<hex<<attr2.value<<"(%ebp),";
                                for(ni=0;ni<4;ni++)
                                {
                                    if(!isBusy[ni])
                                        break;
                                }
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax"<<endl;
                                    outFile<<"subl $0x"<<hex<<newattr.message[1]<<",%eax"<<endl;
                                    break;
                                    case 1:
                                    outFile<<"%ebx"<<endl;
                                    outFile<<"subl $0x"<<hex<<newattr.message[1]<<",%ebx"<<endl;
                                    break;
                                    case 2:
                                    outFile<<"%ecx"<<endl;
                                    outFile<<"subl $0x"<<hex<<newattr.message[1]<<",%ecx"<<endl;
                                    break;
                                    case 3:
                                    outFile<<"%edx"<<endl;
                                    outFile<<"subl $0x"<<hex<<newattr.message[1]<<",%edx"<<endl;
                                    break;
                                }
                                isBusy[ni] = 1;
                            }
                        }
                        else if(newattr.message[4] == 1)//是一个变量
                        {
                            outFile<<"movl ";
                            if(!strcmp(attr2.str,"array") && attr2.message[4]==1)//!!这是一个以变量为下标的数组
                            {
                                for(ni=0;ni<4;ni++)
                                {
                                    if(!isBusy[ni])
                                        break;
                                }
                                isBusy[ni] = 1;
                                outFile<<"movl -0x"<<hex<<newattr.message[1]<<"(%ebp),";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax"<<endl;
                                    break;
                                    case 1:
                                    outFile<<"%ebx"<<endl;
                                    break;
                                    case 2:
                                    outFile<<"%ecx"<<endl;
                                    break;
                                    case 3:
                                    outFile<<"%edx"<<endl;
                                    break;
                                }
                                addr = newattr.value;
                                outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax";
                                    break;
                                    case 1:
                                    outFile<<"%ebx";
                                    break;
                                    case 2:
                                    outFile<<"%ecx";
                                    break;
                                    case 3:
                                    outFile<<"%edx";
                                    break;
                                }
                                outFile<<",4),";
                                isBusy[ni] = 0;
                                isBusy[0] = 1;
                            }
                            else if(!strcmp(attr2.str,"var"))//这是一个变量
                            {
                                outFile<<"-0x"<<hex<<attr2.value<<"(%ebp),";
                                for(ni=0;ni<4;ni++)
                                {
                                    if(!isBusy[ni])
                                        break;
                                }
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%eax"<<endl;
                                    break;
                                    case 1:
                                    outFile<<"%ebx"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%ebx"<<endl;
                                    break;
                                    case 2:
                                    outFile<<"%ecx"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%ecx"<<endl;
                                    break;
                                    case 3:
                                    outFile<<"%edx"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%edx"<<endl;
                                    break;
                                }
                                isBusy[ni] = 1;
                            }
                            else if(!strcmp(attr2.str,"num"))//这是一个数字
                            {
                                outFile<<"$0x"<<hex<<attr2.value<<",";
                                for(ni=0;ni<4;ni++)
                                {
                                    if(!isBusy[ni])
                                        break;
                                }
                                switch(ni)
                                {
                                    case 0:
                                    outFile<<"%eax"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%eax"<<endl;
                                    break;
                                    case 1:
                                    outFile<<"%ebx"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%ebx"<<endl;
                                    break;
                                    case 2:
                                    outFile<<"%ecx"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%ecx"<<endl;
                                    break;
                                    case 3:
                                    outFile<<"%edx"<<endl;
                                    outFile<<"subl -0x"<<hex<<newattr.message[1]<<"(%ebp)"<<",%edx"<<endl;
                                    break;
                                }
                                isBusy[ni] = 1;
                            }
                        }
                        strcpy(att.str,"reg");
                        att.value = ni;
                    }
                    break;
                case 8://<B>-><BS>
                    pop1();
                    pop2();
                    break;
                case 63://<FORX>->for ( <FOD> <FOT> <FOF> ) { <B> }
                    pop1();
                    pop2();
                    pop1();
                    pop2();
                    pop1();
                    pop2();
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    if(newattr.message[0] == 1)
                    {
                        outFile<<"addl $0x1,-0x"<<hex<<newattr.message[1]<<"(%ebp)"<<endl;
                    }
                    else if(newattr.message[0] == 2)
                    {
                        outFile<<"subl $0x1,-0x"<<hex<<newattr.message[1]<<"(%ebp)"<<endl;
                    }
                    for(int i =0;i<4;i++)
                    {
                        pop1();
                        pop2();
                    }
                    break;
                case 61://<X>-><FORX>
                    pop1();
                    pop2();
                    outFile<<"jmp L"<<dec<<jumpN-1<<endl;
                    break;
                case 11://<BS>-><X>
                    pop1();
                    pop2();
                    outFile<<"L"<<dec<<jumpN<<":"<<endl;
                    jumpN++;
                    break;
                case 77://<GK>->!=
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"");
                    att.value = newattr.value;
                    break;
                case 73://<T>->( <M> <GK> <M> )
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    //outFile<<newattr.str<<endl;
                    if(!strcmp(newattr.str,"num"))//<M>是数字
                    {
                        att.message[2] = newattr.value;
                        att.message[5] = 0;
                    }
                    else if(!strcmp(newattr.str,"var")){
                        att.message[2] = varlist.addr[newattr.value];
                        //outFile<<att.message[2]<<endl;
                        att.message[5] = 1;
                    }
                    pop1();
                    newattr = pop2();
                    att.message[0] = newattr.value;
                    pop1();
                    newattr = pop2();
                    if(!strcmp(newattr.str,"num"))//<M>是数字
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 0;
                    }
                    else if(!strcmp(newattr.str,"var")){
                        att.message[1] = newattr.value;
                        att.message[4] = 1;
                    }
                    pop1();
                    pop2();
                    outFile<<"L"<<jumpN<<":";
                    att.message[6] = jumpN;
                    jumpN++;
                    outFile<<"cmpl ";
                    if(att.message[5])
                        outFile<<"-0x"<<hex<<att.message[2]<<"(%ebp),";
                    else
                        outFile<<"$0x"<<hex<<att.message[2]<<",";
                    if(att.message[4])
                        outFile<<"-0x"<<hex<<att.message[1]<<"(%ebp)";
                    else
                        outFile<<"$0x"<<hex<<att.message[1];
                    outFile<<endl;
                    switch(att.message[0])
                    {
                        case equal:
                            break;
                        case no_less:
                            outFile<<"jb "<<"L"<<jumpN<<endl;
                            break;
                        case less:
                            outFile<<"jae "<<"L"<<jumpN<<endl;
                            break;
                        case biger:
                            break;
                        case no_biger:
                            break;
                        case no_equal:
                            outFile<<"jz "<<"L"<<jumpN<<endl;
                            break;
                    }
                    break;
                case 51://<MF>->- <M>
                    pop1();
                    newattr = pop2();
                    att.message[0] = 2;
                    if(!strcmp(newattr.str,"reg"))
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 2;
                    }
                    else if(!strcmp(newattr.str,"num"))
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 0;
                    }
                    else if(!strcmp(newattr.str,"var"))
                    {
                        att.message[1] = newattr.value;
                        att.message[4] = 1;
                    }
                    pop1();
                    pop2();
                    break;
                case 18://<Z>->var --
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"var");
                    att.value = varlist.addr[newattr.value];
                    //outFile<<att.value<<endl;
                    att.message[0] = 2;
                    break;
                case 14://<BS>-><Z> ;
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    if(newattr.message[0] == 1)
                    {
                        outFile<<"addl $0x1,-0x"<<hex<<newattr.value<<"(%ebp)"<<endl;
                    }
                    else if(newattr.message[0] == 2)
                    {
                        outFile<<"subl $0x1,-0x"<<hex<<newattr.value<<"(%ebp)"<<endl;
                    }
                    break;
                case 7://<B>-><BS> <B>
                    pop1();
                    pop2();
                    pop1();
                    pop2();
                    break;
                case 83://<WHILEX>->while <T> { <B> }
                    for(int i=0;i<3;i++)
                    {
                        pop1();
                        pop2();
                    }
                    pop1();
                    newattr = pop2();
                    for(int i=0;i<10;i++)
                    {
                        att.message[i] = newattr.message[i];
                    }
                    pop1();
                    pop2();
                    break;
                case 62://<X>-><WHILEX>
                    pop1();
                    newattr = pop2();
                    nt = newattr.message[6];
                    outFile<<"jmp "<<"L"<<dec<<nt<<endl;
                    break;
                case 55://<MS>->/ <MC>
                    pop1();
                    newattr = pop2();
                    if(!strcmp(newattr.str,"array") && newattr.message[4]==1)//这是一个以变量为下标的数组
                    {
                        for(ni=0;ni<4;ni++)
                        {
                            if(!isBusy[ni])
                                break;
                        }
                        isBusy[ni] = 1;
                        outFile<<"movl -0x"<<hex<<newattr.message[1]<<"(%ebp),";
                        switch(ni)
                        {
                            case 0:
                            outFile<<"%eax"<<endl;
                            break;
                            case 1:
                            outFile<<"%ebx"<<endl;
                            break;
                            case 2:
                            outFile<<"%ecx"<<endl;
                            break;
                            case 3:
                            outFile<<"%edx"<<endl;
                            break;
                        }
                        addr = newattr.value;
                        outFile<<"mov -0x"<<hex<<addr<<"(%ebp,";
                        switch(ni)
                        {
                            case 0:
                            outFile<<"%eax";
                            break;
                            case 1:
                            outFile<<"%ebx";
                            break;
                            case 2:
                            outFile<<"%ecx";
                            break;
                            case 3:
                            outFile<<"%edx";
                            break;
                        }
                        for(nt=0;nt<4;nt++)
                        {
                            if(!isBusy[nt])
                                break;
                        }
                        outFile<<",4),";
                        switch(nt)
                        {
                            case 0:
                            outFile<<"%eax"<<endl;
                            break;
                            case 1:
                            outFile<<"%ebx"<<endl;
                            break;
                            case 2:
                            outFile<<"%ecx"<<endl;
                            break;
                            case 3:
                            outFile<<"%edx"<<endl;
                            break;
                        }
                        isBusy[ni] = 0;
                        isBusy[nt] = 1;
                        isBusy[0] = 1;
                        att.message[1] = nt;
                        att.message[4] = 2;
                        att.message[0] = 3;
                    }
                    else if(!strcmp(newattr.str,"var"))
                    {
                        att.message[0] = 4;
                        att.message[1] = newattr.value;
                        att.message[4] = 1;
                    }
                    pop1();
                    pop2();
                    break;
                case 79://<GK>->>=
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,"");
                    att.value = newattr.value;
                    break;
                case 101://<PRA>-><M>
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,newattr.str);
                    att.value = newattr.value;
                    break;
                case 99://<PRLI>-><PRA>
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,newattr.str);
                    att.value = newattr.value;
                    break;
                case 92://<Y>->print ( str , <PRLI> ) ;
                    for(int i=0;i<2;i++)
                    {
                        pop1();
                        pop2();
                    }
                    pop1();
                    newattr = pop2();
                    strcpy(att.str,newattr.str);
                    att.value = newattr.value;
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    nt = newattr.value;
                    varlist.type[nt] = le_string;
                    att.message[1] = newattr.value;
                    for(int i=0;i<2;i++)
                    {
                        pop1();
                        pop2();
                    }
                    break;
                case 13://<BS>-><Y>
                    pop1();
                    newattr = pop2();
                    //coutFile<<newattr.str<<endl;
                    if(!strcmp(newattr.str,"var"))
                    {
                        outFile<<"pushl -0x"<<newattr.value<<"(%ebp)"<<endl;
                        outFile<<"pushl $text"<<newattr.message[1]<<endl;
                        outFile<<"call _printf"<<endl;
                    }
                    break;
                case 86://<IF>->if <T> { <B> }
                    for(int i=0;i<5;i++)
                    {
                        pop1();
                        pop2();
                    }
                    outFile<<"jmp L"<<dec<<jumpN+1<<endl;
                    outFile<<"L"<<jumpN<<":"<<endl;
                    jumpN++;
                    break;
                case 91://<ELSE_E>->{ <B> }
                    for(int i=0;i<3;i++)
                    {
                        pop1();
                        pop2();
                    }
                    break;
                case 88://<ELSE>->else <ELSE_E>
                    pop1();
                    pop2();
                    pop1();
                    pop2();
                    break;
                case 87://<IF_E>-><ELSE>
                    pop1();
                    pop2();
                    break;
                case 84://<W>-><IF> <IF_E>
                    pop1();
                    pop2();
                    pop1();
                    pop2();
                    break;
                case 12://<BS>-><W>
                    pop1();
                    pop2();
                    outFile<<"L"<<dec<<jumpN<<":"<<endl;
                    break;
                case 2://<RET>->return <FH> ;
                    pop1();
                    pop2();
                    pop1();
                    newattr = pop2();
                    outFile<<"movl $0x"<<hex<<newattr.value<<",%eax"<<endl;
                    outFile<<"leave"<<endl<<"ret"<<endl;
                    break;
            }
            /*for(int i=0;i<n;i++)
            {
                pop1();
                pop2();
            }*/
            st = look1();
            //outFile<<"R "<<"st: "<<st<<" Goto: "<<Goto[st][pro]<<" pro: "<<pro<<endl;
            push1(Goto[st][pro]);
            att.key = pro+Base;
            push2(att);
        }
        else if(at == 'E')
        {
            outFile<<"Good Luck!!"<<endl;
            break;
        }
        else
        {
            break;
        }
    }
}
