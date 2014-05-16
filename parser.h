#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer.h"
#include "ast.h"

class parser{
public:
    parser(lexer* l);
    parser();
    ~parser();

    void init(lexer* l);

    void parse();
    ast* get_root();
private:
    ast* block();
    ast* statement();
    ast* condition();
    ast* expression();
    ast* term();
    ast* factor();

    void match_next(PL0_token t);
    void match(PL0_token t,PL0_token r);

    lexer* lex;

    ast* root;
};

#endif
