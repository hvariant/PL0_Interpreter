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
    void match_block_follow(PL0_token token);

    ast* statement();
    void match_statement_follow(PL0_token token);

    ast* condition();
    void match_condition_follow(PL0_token token);

    ast* expression();
    void match_expression_follow(PL0_token token);

    ast* term();
    void match_term_follow(PL0_token token);

    ast* factor();
    void match_factor_follow(PL0_token token);

    void match_next(const char* func,PL0_token t,string& s);
    void match(const char* func,PL0_token t,PL0_token r);

    lexer* lex;

    ast* root;
};

#endif
