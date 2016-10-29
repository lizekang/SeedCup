#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <vector>
using namespace std;

//存放处理后的字符串
char tempstr[255]={};
//空格标志
bool temp=false;
//临时数组
char word[255]={};
//keyword关键字
enum {
    IF,WHILE,DO,INT,BREAK,ELSE,FOR,PRINTF
};
string keyword[9]={"if","while","do","int","break","else","for","printf"};

int keyword_num[9]={1,2,3,4,5,6,7,8,9};
//部分运算符，定界符等
char symbol[12]={'*','/','=',';','(',')','#','{','}',',','%'};
//对应的种码值
int symbol_num[12]={29,30,38,41,42,43,0,111,222,333,444,555};

//判断是否为字母
bool IsLetter(char ch)
{
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
        return true;
    return false;
}

//判断是否为数字
bool IsDigit(char ch)
{
    if(ch>='0'&&ch<='9')
        return true;
    return false;
}

//判断是否为定界符等
int IsSymbol(char ch)
{
    for(int i=0;    i<12;    i++)
    {
        if(ch==symbol[i])
            return i;
    }
    return -1;
}

//判断是否为关键字
int IsKeyword(string instr)
{
    for(int i=0;    i<26;    i++)
    {
        if(instr==keyword[i])
        {
            return i;
        }
    }
    //不是关键字即为ID
    return 25;
}

int main()
{
    char instr[255]={}; //接收输入字符串
    bool flag=false; //多行注释标志,false为未处于注释区域
    string Token;//存放字符串
    char *str=NULL;//存放每行的字符串
    char delims[]=" ";//分割标志
    freopen("test.cpp","r",stdin);
    int line=1;
//    freopen("result.txt","w",stdout); //此行注释后，控制台输出，
    //否则文本输出
    while((gets(instr))!=NULL)
    { 
        if(*(instr)=='#') {
            printf("#\n");
            
            break;
        }
        printf("%d\n",line);
        
        for(int i=0;i<strlen(instr);i++){
            if((*(instr+i)==' ') || (*(instr+i)=='\t') ){
                i++;
            }
            if( (*(instr+i)=='/') && (*(instr+i+1)=='/')){
            break;
        }
                if(*(instr+i)=='/'){
                    if(*(instr+i+1)=='*')
                    {
                        i++;
                        flag=true;
                        continue;
                    }
                }
                //注释处理: */,注释区域结束
                if(*(instr+i)=='*'&&flag){
                    if(*(instr+i+1)=='/'){
                        flag=false;
                        i++;
                        continue;
                    }
                }
                //标识符，关键词
                if(IsLetter(*(instr+i))&&(!flag)){
                    //printf("进入标识符判断\n");
                    while(IsLetter(*(instr+i))||IsDigit(*(instr+i))||*(instr+i)=='_'){
                        Token+=*(instr+i);
                        i++;
                    }
                    if(IsKeyword(Token)!=25){
                        printf("%s---->%d\n",Token.c_str(),keyword_num[IsKeyword(Token)]);
                    }else
                        printf("%s---->25\n",Token.c_str());
                    Token="";
                    //printf("退出标识符判断\n");
                }
                if(IsDigit(*(instr+i))&&(!flag)){
                    //printf("进入数字判断\n");
                    while(IsDigit(*(instr+i))){
                        Token+=*(instr+i);
                        i++;
                    }
                    printf("%s------>26\n",Token.c_str());
                    Token="";
                }
                //+
                if((*(instr+i)=='+') && (*(instr+i+1)!='+')){
                    printf("+ ------>+\n");
                }
                //++
                if((*(instr+i)=='+') && (*(instr+i+1)=='+')){
                    printf("++ ------>444\n");
                    i++;
                }
                //-
                if((*(instr+i)=='-') && ((*(instr+i+1))!='-')){
                    printf("- -------->-\n");
                }
                //--
                if(*(instr+i)=='-'&&(*(instr+i+1))=='-'){
                    printf("-- -------->555\n");
                    i++;
                }
                if(*(instr+i)=='\"'){
                    char *temp =NULL;
                    temp = instr;
                    int s =i;
                    while(*(instr+s)!='\0'){
                        s++;
                        if(*(instr+s)=='\"'){
                            temp = (instr+s);
                        }
                    }
                    while(temp != (instr+i)){
                        Token+=*(instr+i);
                        i++;
                    }
//                    Token+= *(instr+i);
//                    i++;
//                    while((*(instr+i)!='\"') || (*(instr+i)==' ') || ( (*(instr+i)=='\"') &&(*(instr+i-1)=='\\'))){
//                        Token+=*(instr+i);
//                        i++;
//                    }
                    Token+=*(instr+i);
                    i++;
                    printf("%s\n",Token.c_str());
                    Token="";
                }
                //<,<=,<>
                if(*(instr+i)=='<'&&(!flag)){
                    if(*(instr+i)=='='){printf("<=------>35\n");i++;}
                    if(*(instr+i)=='>')    {printf("<>------>34\n");i++;}
                    else printf("<------>33\n");
                }
                    //>,>=
                else if(*(instr+i)=='>'&&(!flag)){
                    if(*(instr+i+1)=='=') {printf(">------>37\n");}
                    else printf(">-------36\n");
                }
                    //:,:=
                else if(*(instr+i)==':'&&(!flag)){
                    if(*(instr+i+1)=='=') {printf(":=------->32\n");}
                    else printf(":-------->31\n");
                }
                    //余下定界符等
                else if(IsSymbol(*(instr+i))!=-1&&(!flag))
                {   //printf("debug");
                    printf("%c------->%d\n",*(instr+i),symbol_num[IsSymbol(*(instr+i))]);
                }
            }
//            instr=strtok(NULL,delims);


        line++;
    }

    return 0;
}
