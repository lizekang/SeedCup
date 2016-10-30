#include "SeedCup.h"
/*全局变量*/
Var *root;
Word *start;
Word *token;
Var *current_scope;
Var *current_var;
Var *temp_var;
int line_number;
int last_line;
int is_assign = 0;

void init(){
    start = lex();
    printf("\n\n\n\n");
    start = start->next;
    root = (Var*)malloc(sizeof(Var));
    root->is_var = FALSE;
    root->parent = root->next = NULL;
    root->scope = NULL;
    current_var = current_scope = root;
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

void next(int is_stop){
    token = token->next;
    if(token == NULL) {
        return;
    }
    line_number = token->line_number;

    if(token->type == INT){
        int_handler(is_stop);
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
    else if(token->type == VAR){
        /*Word *prevtemp,*nexttemp;
        prevtemp = token->prev;
        nexttemp = token->next;
        if(prevtemp->type == COMMA||prevtemp->type == INT){
            string name = token->name;
            save_var(1,name);
        }
        if(nexttemp->type == ASSIGN){
            var_handler();
        }*/
        var_handler(is_stop);
    }
    else if(token->type == PRINTF){
        printf_handler(is_stop);
    }
    else if(token->type == SEMICOLON){
        if(token->prev->type == SEMICOLON || token->prev->type == RBC){
            next(0);
        }
    }
}

void var_handler(int is_stop){
    print_line(token->line_number);
    if(token->next->type == ASSIGN) {
        is_assign = 1;
        string name = token->name;
        token = token->next;
        update_var(get_value(name), expression(1));
        is_assign = 0;
        if(token->type == SEMICOLON){
            if(is_stop == 0)
                next(0);
        }
    }
    else if(is_assign == 0){
        expression(1);
        if(token->type ==SEMICOLON){
            if(is_stop == 0)
                next(0);
        }
    }
}

void int_handler(int is_stop){
    string name;
    print_line(token->line_number);
    while(1){
        token = token->next;
        if(token->type == VAR&&token->next->type == ASSIGN){
            is_assign = 1;
            name = token->name;
            save_var(1,name);
            token = token->next;
            update_var(get_value(name),expression(1));
            is_assign = 0;
            if(token->type == COMMA){
                continue;
            }
            else if(token->type == SEMICOLON){
                break;
            }
        }
    }
    if(is_stop == 0)
        next(0);
}

void printf_handler(int is_stop){
    print_line(token->line_number);
    next(0);
    if(is_stop == 0){
        if(token->type == LC){
            while(token->type != SEMICOLON){
                next(0);
            }
            next(0);
        }
    }
    else{
        if(token->type == LC){
            while(token->type != SEMICOLON){
                next(0);
            }
        }
    }

}

void if_handler() {
    Word *first_word = token;
    token = token->next; //跳过if本身, 到'('处

    print_line(token->line_number);

    if (!expression(0)) {
        //跳过if的域
        if (token->next->type != LBC) {
            while (token->type != SEMICOLON) {
                token = token->next;
            }
        } else {
            jump_through_block();
        }
        //接着查看是否有else或者else if
        if (token != NULL && (token->type == RBC || token->type == SEMICOLON)) {
            if (token->next->type == ELSE) {
                token = token->next;
                if (token->next->type == IF) {
                    token = token->next;
                    if_handler();
                } else {
                    in_scope(first_word); //改变作用域
                    if (token->next->type != LBC) {
                        next(1); //执行else的下一句语句，执行完后token指针应指在'}'或';'处
                    } else {
                        token = token->next; //跳到'{'
                        next(0); //执行else内的语句, 执行完后token指针应指在'}'或';'处
                    }
                    out_scope();
                }
            }
        }
    }
    else {
        in_scope(first_word); //改变作用域
        if (token->next->type != LBC) {
            next(1);
        } else {
            token = token->next; //跳到'{'
            next(0); //执行if内的语句, 执行完后token指针应指在'}'处
        }
        out_scope(); //跳出作用域
        //跳过之后所有的else if和else
        while (token->next->type == ELSE) {
            jump_through_block();
        }
    }
    next(0);
}

void for_handler(){

}

void while_handler(){
    token = token->next;
    /*if(token->type == LC) {
        expression(0);
    }
    if(token->type == RC){
        token = token->next;
        if(token->type == LBC){

            Var *new_scope = (Var*)malloc(sizeof(VAR));
            new_scope->firstWord = token->next;
            new_scope->parent = current_var;
            current_var->scope = new_scope;
            new_scope->is_var = FALSE;

            while(token->type!=RBC){
                next(0);
            }
        }
    }*/
    if(token->type == LC){
        expression(0);
    }
}

void save_var(int type,string name){
    if(current_var->is_var) {
        if (type) {
            Var *temp = (Var *) malloc(sizeof(Var));
            temp->is_var = TRUE;
            temp->name = name;
            temp->parent = current_var;
            current_var->next = temp;
            current_var = temp;
        }
        else {
            Var *temp = (Var *) malloc(sizeof(Var));
            temp->is_var = FALSE;
            temp->parent = current_var;
            current_var->next = temp;
            current_var = temp;
        }
    }
    else{
        if (type) {
            Var *temp = (Var *) malloc(sizeof(Var));
            temp->is_var = TRUE;
            temp->name = name;
            temp->parent = current_var;
            current_var->scope = temp;
            current_var = temp;
        }
        else {
            Var *temp = (Var *) malloc(sizeof(Var));
            temp->is_var = FALSE;
            temp->parent = current_var;
            current_var->scope = temp;
            current_var = temp;
        }
    }
}

void update_var(Var *var,int result){
    var->value = result;
}

void do_while_handler(){

}

Var* get_value(string name){
    Var *temp = current_var;
    while(temp->parent!=NULL) {
        if(temp->name == name){
            return temp;
        }
        else{
            temp = temp->parent;
        }
    }
    return NULL;
}

//void match(string tk) {
//    if (token->name == tk) {
//        next();
//    } else {
//        printf("%d: expected token: %s\n", token->line_number, tk);
//        return;
//    }
//}

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

int expression(int type) {
    if (type) {
        string expr = "";
        next(0);
        int flag = 0;
        while ((token->type == VAR) || (token->type == NUMBER) || (token->type == MATHOP)
               || (token->type == LC) || (token->type == RC) || (token->type == INC)) {
            if (token->type == VAR) {
                if (is_have_var(token->name)) {
                    expr += to_string(get_value(token->name)->value);
                }
            }
            else if (token->type == INC) {
                Word *prev_token = token->prev;
                if (is_have_var(prev_token->name)) {
                    Var *var = get_value(prev_token->name);
                    expr += to_string(var->value);
                    update_var(var, var->value+1);
                }
            }
            else {
                if (token->type == LC) {
                    flag++;
                } else if (token->type == RC) {
                    flag--;
                    if (flag == 0) {
                        break;
                    }
                }
                expr += token->name;
            }

            next(0);
        }
        char c_expr[255];
        strcpy(c_expr, expr.c_str());
        return mathOperator(c_expr);
    }
    else {
        int result = 0;
        int a = expression(1);
        if (token->type != COMOP && token->type != ASSIGN) {
            result = a;
        }
        else {
            Word *prev_token = token->prev;
            string assign = token->name;
            int b = expression(1);

            if (assign == ">") {
                if (a > b) {
                    result = 1;
                }
            } else if (assign == "<") {
                if (a < b) {
                    result = 1;
                }
            } else if (assign == "==") {
                if (a == b) {
                    result = 1;
                }
            } else if (assign == ">=") {
                if (a >= b) {
                    result = 1;
                }
            } else if (assign == "<=") {
                if (a <= b) {
                    result = 1;
                }
            } else if (assign == "=") {
                if (is_have_var(prev_token->name)) {
                    Var *var = get_value(prev_token->name);
                    update_var(var, b);
                }
                result = 1;
            }
        }

        if (token->type == COMMA) {
            result = expression(0);
        }

        return result;
    }
}

void print_line(int line){
    if(line!=last_line){
        printf("%d ",line);
        last_line = line;
    }
}

void in_scope(Word *first_word) {
    Var *new_scope = (Var*)malloc(sizeof(Var));
    new_scope->is_var = FALSE;
    new_scope->parent = current_var;
    new_scope->next = NULL;
    new_scope->firstWord = first_word;

    current_var->next = new_scope;
    current_var = new_scope;
    current_scope = new_scope;
}

void out_scope() {
    Var *scope_temp = current_scope->parent;
    Var *var_temp = current_scope->parent;
    free_scope_var();

    while (scope_temp->is_var != 0) {
        scope_temp = scope_temp->parent;
    }

    current_scope = scope_temp;
    free(current_var);
    current_var = var_temp;
    current_var->next = NULL;
}

void free_scope_var() {
    Var *temp1 = current_var;
    while (temp1->is_var != 0) {
        Var *temp2 = temp1->next;
        free(temp1);
        temp1 = temp2;
    }
    current_var = temp1;
    current_var->next = NULL;
}

void jump_through_block() {
    int flag = 0;
    while (1) {
        if (token->type == LBC) {
            flag++;
        } else if (token->type == RBC) {
            flag--;
            if (flag == 0) {
                break;
            }
        }
        token = token->next;
    }
}

int main(){
    init();
    next(0);
//    string sname;
//    int a = 1;
//    sname = "1+(-1)";
//    const char *cname=sname.c_str();
//    char ccname[500];
//    strcpy(ccname, cname);
//    int result = mathOperator(ccname);
//    printf("%d",result);
    return 0;
}


