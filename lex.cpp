
#include"lex.h"
string keyword[8]={"if","while","do","int","break","else","for","printf"};

int keyword_num[9]={IF,WHILE,DO,INT,BREAK,ELSE,FOR,PRINTF};
//部分运算符，定界符等
char symbol[9]={'*','/','=',';','(',')','{','}',','};
//对应的种码值
int symbol_num[9]={MATHOP,MATHOP,ASSIGN,SEMICOLON,LC,RC,LBC,RBC,COMMA};

//添加Token
Position AddToken(string name,int type,int line  ){
    Position NewToken = (Position)malloc(LEN);
    NewToken->name = name;
    NewToken->type = type;
    NewToken->line_number = line;
    return NewToken;
}
//判断是否为字母
bool IsLetter(char ch)
{
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='_'))
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
int IsKeyword(string str)
{
    for(int i=0;    i<8;    i++)
    {
        if(str==keyword[i])
        {
            return i;
        }
    }
    //不是关键字即为ID
    return 8;
}
//词法分析器
Position lex() {
    Position HEAD,Last,Temp;
    HEAD = (Position)malloc(LEN);
    Last = HEAD;
    HEAD->prev = NULL;
    char instr[255]={}; //接收输入字符串
    bool flag=false; //多行注释标志,false为未处于注释区域
    string Token;//存放字符串
    char *str=NULL;
    freopen("/Users/yifan/seedcup/SeedCup/test.cpp","r",stdin);
    int line=1;
//    freopen("result.txt","w",stdout); //此行注释后，控制台输出，
    //否则文本输出
    while((gets(instr))!=NULL) {
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
                    if(IsKeyword(Token)!=8){
                        Temp = AddToken(Token,keyword_num[IsKeyword(Token)],line);
                        Last->next= Temp;
                        Temp->prev = Last;
                        Last=Temp;
                        printf("%s---->%d\n",Token.c_str(),keyword_num[IsKeyword(Token)]);
                        printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    }else {
                        Temp = AddToken(Token,VAR,line);
                        Last->next= Temp;
                        Temp->prev = Last;
                        Last=Temp;
                        printf("%s---->25\n", Token.c_str());
                        printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    }
                    Token="";
                    //printf("退出标识符判断\n");
                }
                if(IsDigit(*(instr+i))&&(!flag)){
                    //printf("进入数字判断\n");
                    while(IsDigit(*(instr+i))){
                        Token+=*(instr+i);
                        i++;
                    }
                    Temp = AddToken(Token,NUMBER,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    printf("%s------>26\n",Token.c_str());
                    printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    Token="";
                }
                //+
                if((*(instr+i)=='+') && (*(instr+i+1)!='+')){
                    Temp = AddToken("+",MATHOP,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    printf("+ ------>+\n");
                    printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                }
                //++
                if((*(instr+i)=='+') && (*(instr+i+1)=='+')){
                    Temp = AddToken("++",RNC,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    printf("++ ------>444\n");
                    printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    i++;
                }
                //-
                if((*(instr+i)=='-') && ((*(instr+i+1))!='-')){
                    Temp = AddToken("-",MATHOP,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    printf("- -------->-\n");
                    printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                }
                //--
                if(*(instr+i)=='-'&&(*(instr+i+1))=='-'){
                    Temp = AddToken("--",INC,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    printf("-- -------->555\n");
                    printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    i++;
                }
                if(*(instr+i)=='\"'){
                    char *temp_string =NULL;
                    temp_string = instr;
                    int s =i;
                    while(*(instr+s)!='\0'){
                        s++;
                        if(*(instr+s)=='\"'){
                            temp_string = (instr+s);
                        }
                    }
                    while(temp_string != (instr+i)){
                        Token+=*(instr+i);
                        i++;
                    }
                    Token+=*(instr+i);
                    i++;
                    Temp = AddToken(Token,STRING,line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    printf("%s\n",Token.c_str());
                    printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    Token="";
                }
                //<,<=,<>
                if(*(instr+i)=='<'&&(!flag)){
                    if(*(instr+i)=='='){
                        Temp = AddToken("<=",COMOP,line);
                        Last->next= Temp;
                        Temp->prev = Last;
                        Last=Temp;
                        printf("<=------>35\n");
                        printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                        i++;
                    }
                    if(*(instr+i)=='>')    {
                        Temp = AddToken("<>",COMOP,line);
                        Last->next= Temp;
                        Temp->prev = Last;
                        Last=Temp;
                        printf("<>------>34\n");
                        printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                        i++;
                    }
                    else {
                        Temp = AddToken("<",COMOP,line);
                        Last->next= Temp;
                        Temp->prev = Last;
                        Last=Temp;
                        printf("<------>33\n");
                        printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    }
                }
                    //>,>=
                else if(*(instr+i)=='>'&&(!flag)){
                    if(*(instr+i+1)=='=') {
                        Temp = AddToken(">=",COMOP,line);
                        Last->next= Temp;
                        Temp->prev = Last;
                        Last=Temp;
                        printf(">------>37\n");
                        printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    }
                    else {
                        Temp = AddToken(">",COMOP,line);
                        Last->next= Temp;
                        Temp->prev = Last;
                        Last=Temp;
                        printf(">-------36\n");
                        printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    }
                }
                    //余下定界符等
                else if(IsSymbol(*(instr+i))!=-1&&(!flag)) {
                    Token+=*(instr+i);
                    Temp = AddToken(Token,symbol_num[IsSymbol(*(instr+i))],line);
                    Last->next= Temp;
                    Temp->prev = Last;
                    Last=Temp;
                    printf("%c------->%d\n",*(instr+i),symbol_num[IsSymbol(*(instr+i))]);
                    printf("Token {%d, line:%d,%s }\n",Last->type,Last->line_number,Last->name.c_str());
                    Token="";
                }
            }
        line++;
    }

    return HEAD;
}
int main(){
    Position Last = lex();
    printf("\n\n\n");
    Last = Last->next;
    while(Last->next!=NULL){
        printf("Token {%d,   line:%d,     %s }\n",Last->type,Last->line_number,Last->name.c_str());
        Last = Last->next;
    }
    printf("Token {%d,   line:%d,     %s }\n",Last->type,Last->line_number,Last->name.c_str());

    return 0;
}
