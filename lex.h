#ifndef SEEDCUP_LEX_H
#define SEEDCUP_LEX_H
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;
#define LEN sizeof(struct word)

typedef struct word *Position;
typedef struct word{
    int type;
    string name;
    int line_number;
    Position next;
    Position prev;
}Word;
enum {
    INT,VAR,MATHOP,ASSIGN,COMOP,WHILE,FOR,DO,IF,ELSE,SEMICOLON,BREAK,LBC,RBC,COMMA,LC,RC,PRINTF,NUMBER,INC,DEC,STRING,NOTKNOWN
};
bool IsLetter(char ch);
bool IsDigit(char ch);
int IsSymbol(char ch);
int IsKeyword(string str);
Position AddToken(string name,int type,int line  );
Position lex();
#endif //SEEDCUP_LEX_H
