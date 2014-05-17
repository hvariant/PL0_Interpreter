#include "lexer.h"
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

typedef std::pair<PL0_token,std::string> S_type;

lexer::lexer(char* fn){
    init(fn);
}

lexer::lexer(FILE* fp){
    init(fp);
}

lexer::lexer(){
    input = NULL;
}

lexer::~lexer(){
    if(input) fclose(input);
}

void lexer::init(char* fn){
    FILE* fp = fopen(fn,"r");
    if(!fp){
        fprintf(stderr,"[lexer] Cannot open file %s\n",fn);
        exit(1);
    }

    init(fp);
}

void lexer::init(FILE* fp){
    assert(fp);

    input = fp;

    lineno = 1;
    linepos = 1;
}

int lexer::get_char(){
    char c = fgetc(input);
    if(c == EOF){
        return EOF;
    }
    linepos++;

    if(isspace(c)){
        while(isspace(c)){
            if(c == '\n'){
                lineno++;
                linepos = 1;
            }

            c = fgetc(input);
            if(c == EOF){
                return EOF;
            }
            linepos++;
        }
        ungetc(c,input);

        return ' ';
    }

    return tolower(c);
}

void lexer::unget_char(char c){
    ungetc(c,input);
    linepos--;
}

int lexer::peek(){
    char c = get_char();
    unget_char(c);

    return c;
}

void lexer::match(char c){
    char next = get_char();
    if(next != c){
        if(next != EOF){
            fprintf(stderr,"[lexer] unexpected character %c, expected %c, in line %d:%d\n",next,c,lineno,linepos);
        } else {
            fprintf(stderr,"[lexer] unexpected end of file, expected %c, in line %d:%d\n",c,lineno,linepos);
        }

        exit(1);
    }
}

void lexer::unget_token(PL0_token token, const string& buf){
    S.push(S_type(token, buf));
}

PL0_token lexer::get_token(string& s){
    s.clear();

    if(S.size() > 0){
        S_type s_top = S.top();
        S.pop();

        s = s_top.second;
        return s_top.first;
    }

begin:
    char c = get_char();

    if(c == EOF){
        return PL0_EOF;
    }

    PL0_token ret;

    switch(c){
        case ':':
        {
            match('=');

            s = ":=";
            ret = PL0_ASSIGN;
            break;
        }
        case '.':
        {
            s = ".";
            ret = PL0_PERIOD;
            break;
        }
        case ',':
        {
            s = ",";
            ret = PL0_COMMA;
            break;
        }
        case ';':
        {
            s = ";";
            ret = PL0_SEMICOLON;
            break;
        }
        case '=':
        {
            s = "=";
            ret = PL0_EQ;
            break;
        }
        case '#':
        {
            s = "#";
            ret = PL0_COMPARE;
            break;
        }
        case '<':
        {
            if(peek() == '='){
                match('=');

                s = "<=";
                ret = PL0_COMPARE;
            } else {
                s = "<";
                ret = PL0_COMPARE;
            }
            break;
        }
        case '>':
        {
            if(peek() == '='){
                match('=');

                s = ">=";
                ret = PL0_COMPARE;
            } else {
                s = ">";
                ret = PL0_COMPARE;
            }
            break;
        }
        case '+':
        {
            s = "+";
            ret = PL0_PLUS;
            break;
        }
        case '-':
        {
            s = "-";
            ret = PL0_MINUS;
            break;
        }
        case '*':
        {
            s = "*";
            ret = PL0_TIMES;
            break;
        }
        case '/':
        {
            s = "/";
            ret = PL0_DIV;
            break;
        }
        case '(':
        {
            s = "(";
            ret = PL0_LPAREN;
            break;
        }
        case ')':
        {
            s = ")";
            ret = PL0_RPAREN;
            break;
        }
        case '!':
        {
            s = "!";
            ret = PL0_WRITE;
            break;
        }
        case '?':
        {
            s = "?";
            ret = PL0_READ;
            break;
        }
        default:
        {
            if(isdigit(c)){
                s.push_back(c);
                while(isdigit(peek())){
                    c = get_char();
                    s.push_back(c);
                }

                ret = PL0_NUMBER;
            } else if(isalpha(c)){
                s.push_back(c);
                char nc = peek(); 
                while(isalpha(nc) || nc == '_' || isdigit(nc)){
                    c = get_char();
                    s.push_back(c);

                    nc = peek();
                }

                if(!strcmp(s.c_str(),"const")){
                    ret = PL0_CONST;
                } else if(!strcmp(s.c_str(),"var")){
                    ret = PL0_VAR;
                } else if(!strcmp(s.c_str(),"procedure")){
                    ret = PL0_PROCEDURE;
                } else if(!strcmp(s.c_str(),"call")){
                    ret = PL0_CALL;
                } else if(!strcmp(s.c_str(),"begin")){
                    ret = PL0_BEGIN;
                } else if(!strcmp(s.c_str(),"end")){
                    ret = PL0_END;
                } else if(!strcmp(s.c_str(),"if")){
                    ret = PL0_IF;
                } else if(!strcmp(s.c_str(),"then")){
                    ret = PL0_THEN;
                } else if(!strcmp(s.c_str(),"while")){
                    ret = PL0_WHILE;
                } else if(!strcmp(s.c_str(),"do")){
                    ret = PL0_DO;
                } else if(!strcmp(s.c_str(),"odd")){
                    ret = PL0_ODD;
                } else {
                    ret = PL0_ID;
                }
            } else if(isspace(c)){
                goto begin;
            } else {
                fprintf(stderr,"[lexer] unknown character in %d:%d\n",lineno,linepos);

                exit(1);
            }

            break;
        }
    }

    return ret;
}
