
#include "SeedCup.h"

/*全局变量*/
Var *root;
Word *start;
Word *token;
Var *current_scope;
Var *current_var;
Var *temp_var;
int line_number;


void init(){
    root->is_var = FALSE;
    root->parent = NULL;
    current_var = root;
    token = start;
}

int mycmp(char a, char b) {
    if(b == '(')
        return 1;//左括号直接入栈
    else if((b == '*' || b == '/') &&(a == '+' || a == '-' || a == '('))
        return 1;//*、/优先级高于+、-、(，入栈
    else if((b == '+' || b == '-') && (a == '('))
        return 1;//+、-优先级高于(，入栈
    else
        return 0;
}
/*中缀表达式转后缀表达式
 中缀表达式之间无分割
 后缀表达式操作数、操作符之间用空格分割，便于区分不同操作数*/
void infix_to_suffix(char* infix, char* suffix) {
    int i, k, j=0, top=0;
    char stack[1000];//存储运算符的栈

    for(i=0; infix[i]!='\0'; i++) {
        if(infix[i] >= '0' && infix[i] <= '9') {
            suffix[j++] = infix[i];//操作数则直接输出
        } else {
            if(i != 0 && infix[i-1] >= '0' && infix[i-1] <= '9') {
                suffix[j++] = ' ';//操作数后补充空格分割
            }
            if(infix[i] == ')') {
                //遇到右括号则一直弹出直到左括号，但左括号不输出
                while(stack[top-1] != '(') {
                    suffix[j++] = stack[--top];
                    suffix[j++] = ' ';
                }
                top--;
            } else if(top == 0 || mycmp(stack[top-1], infix[i])) {
                //栈为空或当前操作符的优先级高于栈顶操作符，当前操作符入栈
                stack[top++] = infix[i];
            } else {
                //当前操作符优先级等于或低于栈顶操作符则弹出栈顶
                while(!mycmp(stack[top-1], infix[i])) {
                    suffix[j++] = stack[--top];
                    suffix[j++] = ' ';
                    if(top == 0)
                        break;
                }
                stack[top++] = infix[i];//当前操作符入栈
            }
        }
    }
    //补充空格分割
    if(suffix[j-1] != ' ') {
        suffix[j++] = ' ';
    }
    //如果操作符栈不为空，弹出所有操作符
    while(top != 0) {
        suffix[j++] = stack[--top];
        suffix[j++] = ' ';
    }
    suffix[j] = '\0';
}
/*后缀表达式求值*/
int suffix_value(char* suffix) {
    int i, j;
    char op;
    int stack[1000];
    int top = 0, value = 0;
    for(i=0; suffix[i] != '\0'; i++) {
        if(suffix[i] >= '0' && suffix[i] <= '9') {
            value = value*10 + suffix[i] - '0';
        } else if(suffix[i] == ' ') {
            //操作数入栈
            stack[top++] = value;
            value = 0;
        } else {
            //根据操作符，对栈顶两个操作数进行计算并得到结果
            switch(suffix[i]) {
                case '+': value = stack[top-2] + stack[top-1];break;
                case '-': value = stack[top-2] - stack[top-1];break;
                case '*': value = stack[top-2] * stack[top-1];break;
                case '/': value = stack[top-2] / stack[top-1];break;
                default: break;
            }
            top -= 2;
        }
    }

    return stack[0];
}

int mathOperator(char *expr){
    char suffix[1000];
    infix_to_suffix(expr,suffix);
    return suffix_value(suffix);
}
void next(){
    token = token->next;
    line_number = token->line_number;
    if(token)
    if(token->type == INT){
        int_handler();
    }
    else if(token->type == FOR){
        for_handler();
    }
    else if(token->type == WHILE){
        while_handler();
    }
    else if(token->type == DO){
        do_while_handler();
    }
    else if(token->type == IF){
        if_handler();
    }
    else if(token->type == ELSE){

    }


}
void int_handler(){
    while(token->type!=SEMICOLON){
        next();
        if(token->type == VAR){

        }
    }
}
void if_handler(){

}

void for_handler(){

}

void while_handler(){

}

void save_var(int type,char *name,char *expr){
    if(type == TRUE){
        Var *temp = (Var*)malloc(sizeof(Var));
        temp->is_var = TRUE;
        temp->name = name;
        temp->value = mathOperator(expr);
        temp->parent = current_var;
        current_var->next = temp;
        current_var = temp;
    }
}
void update_handler(){

}
void do_while_handler(){

}
int get_value(char *name){
    Var *temp = current_var;
    while(temp->parent!=NULL) {
        if(hashfunc(name)==temp->hash){
            return temp->value;
        }
        else{
            temp = temp->parent;
        }
    }
}
int hashfunc(char *name){

}
void match(string tk) {
    if (token->name == tk) {
        next();
    } else {
        printf("%d: expected token: %s\n", token->line_number, tk);
        return;
    }
}
int is_have_var(string name){
    Var *temp = current_var;
    int flag = 0;
    while(temp->parent!=NULL){
        if(temp->name == name){
            flag = 1;
            temp_var = temp;
            break;
        }
        else{
            temp = temp->next;
        }
    }
    return flag;
}




int main(){
    //current_line = start;
    //next();
    while(token->next!=NULL){
        next();
    }
    string sname;
    const char *cname=sname.c_str();
    char ccname[500];
    strcpy(ccname, cname);
    int result = mathOperator(ccname);
    printf("%d",result);
    return 0;
}


