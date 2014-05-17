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

    AST_READ,
    AST_WRITE,

    AST_ASSIGN,
    AST_CALL,
    AST_IF,
    AST_WHILE,

    AST_PROGRAM,
    AST_BLOCK,
    AST_STATEMENT,
    AST_CONDITION,
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
    ast* get_child(int i) const;
    void add_child(ast* t){ children.push_back(t); }

    void print(FILE* fp) const;

    void print_stdout() const{ print(stdout); }

    static const char* get_ast_type_map(ast_type);
private:
    static void print_ast(FILE* fp,const ast* t,int indent);

    std::vector<ast*> children;

    ast_type _type;
    PL0_token _token;
    char* sym;
};

#endif
