#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdio.h>
#include "tokens.h"

#define LEXER_BSIZE 100

class lexer{
public:
    lexer(char* fn);
    lexer(FILE* fp);

    lexer();
    ~lexer();

    void init(char* fn);
    void init(FILE* fp);

    void set_bufsize(size_t n);
    size_t get_bufsize();

    PL0_token get_token();
    const char* get_sym();
private:
    FILE* input;

    char* buffer;
    size_t bsize;

    int get_char();
    void unget_char(char c);
    int peek();
    void match(char c);

    int lineno;
    int linepos;
};

#endif
