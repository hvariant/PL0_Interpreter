#ifndef __TOKENS_H__
#define __TOKENS_H__

typedef enum{
    PL0_CONST = 1,
    PL0_VAR,
    PL0_PROCEDURE,
    PL0_CALL,

    PL0_ID,
    PL0_NUMBER,

    PL0_COMMA,
    PL0_PERIOD,
    PL0_LPAREN,
    PL0_RPAREN,
    PL0_SEMICOLON,

    PL0_ASSIGN,
    PL0_PLUS,
    PL0_MINUS,
    PL0_TIMES,
    PL0_DIV,

    PL0_COMPARE,
    PL0_ODD,

    PL0_BEGIN,
    PL0_END,
    PL0_IF,
    PL0_THEN,
    PL0_WHILE,
    PL0_DO,

    PL0_EOF,

    PL0_NONTERM,

    PL0_TOKEN_NUM,
    PL0_EQ, //special virtual token
} PL0_token;

const char* get_token_map(PL0_token t);

#endif
