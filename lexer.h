#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdio.h>
#include <stack>
#include <string>
#include "tokens.h"

using std::string;

class lexer{
public:
    lexer(char* fn);
    lexer(FILE* fp);

    lexer();
    ~lexer();

    void init(char* fn);
    void init(FILE* fp);

    void unget_token(PL0_token token,const string&);

    PL0_token get_token(string& s);

    inline void get_file_pos(int* lineno,int* linepos){
        *lineno = this->lineno;
        *linepos = this->linepos;
    }
private:
    FILE* input;

    std::stack<std::pair<PL0_token, std::string> > S;

    int get_char();
    void unget_char(char c);
    int peek();
    void match(char c);

    int lineno;
    int linepos;
};

#endif
