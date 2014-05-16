#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdio.h>
#include <stack>
#include <string>
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

    void unget_token(PL0_token token);

    PL0_token get_token();
    const char* get_sym();

    inline void get_file_pos(int* lineno,int* linepos){
        *lineno = this->lineno;
        *linepos = this->linepos;
    }
private:
    FILE* input;

    char* buffer;
    size_t bsize;

    std::stack<std::pair<PL0_token, std::string> > S;

    int get_char();
    void unget_char(char c);
    int peek();
    void match(char c);

    int lineno;
    int linepos;
};

#endif
