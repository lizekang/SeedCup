//
// Created by lzk on 10/28/16.
//

#ifndef SEEDCUP_SEEDCUP_H
#define SEEDCUP_SEEDCUP_H
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
using namespace std;
#define BOOL int
#define TRUE 1
#define FALSE 0
enum{
    INT,VAR,MATHOP,ASSIGN,COMOP,WHILE,FOR,DO,IF,ELSE,SEMICOLON,BREAK
};//依次为变量类型,四则运算符,等号,比较运算符,
/*word节点,包含类型和名称*/
typedef struct word{
    int type;
    string name;
    int line_number;
    struct word *next;
    struct word *prev;
}Word;


typedef struct var_node{
    string name;
    int hash;
    BOOL is_var;
    union {
        int value;
        struct var_node *scope;
    };
    Word *firstWord;
    BOOL is_not_cycle;
    struct var_node *next;
    struct var_node *parent;
}Var;


void int_handler();
void init();
int mathOperator(char *expr);
void int_handler();
void if_handler();
void for_handler();
void while_handler();
void do_while_handler();
void save_var(int type,char *name,char *expr);
void update_var();
void out_scope();
int get_value(char *name);
int hashfunc(char *name);
void match(string tk);
int is_have_var(string name);
#endif //SEEDCUP_SEEDCUP_H
