#ifndef __AST_H__
#define __AST_H__

#include <vector>
#include <stdio.h>
#include "tokens.h"

typedef enum{
    AST_TERMINAL = 1,
    AST_CONST,
    AST_VAR,
    AST_PROCEDURE,
    AST_ODD,
    AST_POSITIVE,
    AST_NEGATIVE,

    AST_PLUS,
    AST_MINUS,
    AST_TIMES,
    AST_DIV,

    AST_ASSIGN,
    AST_CALL,
    AST_IF,
    AST_WHILE,

    AST_PROGRAM,
    AST_BLOCK,
    AST_STATEMENT,
    AST_CONDITION,
    AST_EXPRESSION,
    AST_TERM,
} ast_type;

class ast{
public:
    ast(ast_type typ,PL0_token tok = PL0_NONTERM, const char* s = NULL);
    ~ast();

    void init(ast_type typ,PL0_token tok, const char* s);

    ast_type type() const{ return _type; }
    PL0_token token() const{ return _token; }
    const char* term_sym() const{ return sym; }

    int children_size() const{ return children.size(); }
    ast* get_child(int i);
    void add_child(ast* t){ children.push_back(t); }

    void print(FILE* fp);
private:
    std::vector<ast*> children;
    ast_type _type;
    PL0_token _token;
    char* sym;
};

#endif
