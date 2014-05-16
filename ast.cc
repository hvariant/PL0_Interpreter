#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

ast::ast(ast_type typ,PL0_token tok, const char* s){
    init(typ,tok,s);
}

ast::~ast(){
    for(int i=0;i<children.size();i++){
        delete children[i];
    }

    if(sym)
        delete sym;
}

void ast::init(ast_type typ,PL0_token tok,const char* s){
    _type = typ;
    _token = tok;
    if(s){
        sym = strdup(s);
    } else {
        sym = NULL;
    }
}

ast* ast::get_child(int i){
    assert(0 <= i && i < children.size());
    return children[i];
}

const char* ast::get_ast_type_map(ast_type t){
    switch(t){
        case AST_TERMINAL: return "AST_TERMINAL";
        case AST_CONST: return "AST_CONST";
        case AST_VAR: return "AST_VAR";
        case AST_PROCEDURE: return "AST_PROCEDURE";
        case AST_ODD: return "AST_ODD";
        case AST_POSITIVE: return "AST_POSITIVE";
        case AST_NEGATIVE: return "AST_NEGATIVE";

        case AST_PLUS: return "AST_PLUS";
        case AST_MINUS: return "AST_MINUS";
        case AST_TIMES: return "AST_TIMES";
        case AST_DIV: return "AST_DIV";

        case AST_ASSIGN: return "AST_ASSIGN";
        case AST_CALL: return "AST_CALL";
        case AST_IF: return "AST_IF";
        case AST_WHILE: return "AST_WHILE";

        case AST_PROGRAM: return "AST_PROGRAM";
        case AST_BLOCK: return "AST_BLOCK";
        case AST_STATEMENT: return "AST_STATEMENT";
        case AST_CONDITION: return "AST_CONDITION";
        case AST_EXPRESSION: return "AST_EXPRESSION";
        case AST_TERM: return "AST_TERM";
    }
}

void ast::print(FILE* fp){
    print_ast(fp,this,0);
}

static void spaces(FILE* fp,int n){
    for(int i=0;i<n;i++){
        fprintf(fp," ");
    }
}

void ast::print_ast(FILE* fp,ast* t,int indent){
    spaces(fp,indent);

    fprintf(fp,"[%s]:",get_ast_type_map(t->type()));
    if(t->type() == AST_TERMINAL){
        fprintf(fp,"%s[%s]", t->sym, get_token_map(t->token()));
    }
    fprintf(fp,"\n");

    for(int i=0;i<t->children_size();i++){
        print_ast(fp,t->get_child(i),indent+4);
    }
}
