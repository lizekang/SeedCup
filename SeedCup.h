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
#define TRUE 1
#define FALSE 0
enum{
    INT,VAR,MATHOP,ASSIGN,COMOP,WHILE,FOR,DO,IF,ELSE,SEMICOLON,BREAK,LBC,RBC,COMMA,INC,LC,RC,PRINTF,NUMBER,RNC,DEC,STRING
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
    int is_var;
    union {
        int value;
        struct var_node *scope;
    };
    Word *firstWord;
    int is_not_cycle;
    struct var_node *next;
    struct var_node *parent;
}Var;


void init();
void next(int is_stop);

int mathOperator(char *expr);

void int_handler(int is_stop);
void var_handler(int is_stop);
void printf_handler(int is_stop);
void if_handler();
void for_handler();
void while_handler();
void do_while_handler();

void save_var(int type,string name);
void update_var(Var* var,int result);
Var* get_value(string name);
int is_have_var(string name);

void in_scope(Word *first_word);
void out_scope();
void free_scope_var();

int expression(int type);

void jump_through_block();
void print_line(int line);
#endif //SEEDCUP_SEEDCUP_H
