#include "lexer.h"
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

lexer::lexer(char* fn){
    init(fn);
}

lexer::lexer(FILE* fp){
    init(fp);
}

lexer::lexer(){
    input = NULL;
    buffer = NULL;
    bsize = 0;
}

lexer::~lexer(){
    if(buffer) free(buffer);
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

    buffer = NULL;
    set_bufsize(LEXER_BSIZE);

    lineno = 1;
    linepos = 1;
}

void lexer::set_bufsize(size_t n){
    if(buffer){
        buffer = (char*)realloc(buffer,n);
    } else {
        buffer = (char*)malloc((n+10)*sizeof(char));
    }

    if(!buffer){
        fprintf(stderr,"[lexer] error when allocating buffer\n");
        exit(1);
    }
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

    return c;
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
            fprintf(stderr,"[lexer] unexpected end of file, expected %c, in line %d:%d\n",next,c,lineno,linepos);
        }

        exit(1);
    }
}

void lexer::unget_token(PL0_token token){ //@TODO: implement

}

PL0_token lexer::get_token(){
begin:
    char c = get_char();

    if(c == EOF){
        return PL0_EOF;
    }

    PL0_token ret;

    switch(c){
        case '=':
        {
            strcpy(buffer,"=");
            ret = PL0_EQ;
            break;
        }
        case ':':
        {
            match('=');
            strcpy(buffer,":=");
            ret = PL0_ASSIGN;
            break;
        }
        case '.':
        {
            strcpy(buffer,".");
            ret = PL0_PERIOD;
            break;
        }
        case ',':
        {
            strcpy(buffer,",");
            ret = PL0_COMMA;
            break;
        }
        case ';':
        {
            strcpy(buffer,";");
            ret = PL0_SEMICOLON;
            break;
        }
        case '#':
        {
            strcpy(buffer,"#");
            ret = PL0_NEQ;
            break;
        }
        case '<':
        {
            if(peek() == '='){
                match('=');

                strcpy(buffer,"<=");
                ret = PL0_LE;
            } else {
                strcpy(buffer,"<");
                ret = PL0_LT;
            }
            break;
        }
        case '>':
        {
            if(peek() == '='){
                match('=');

                strcpy(buffer,">=");
                ret = PL0_GE;
            } else {
                strcpy(buffer,">");
                ret = PL0_GT;
            }
            break;
        }
        case '+':
        {
            strcpy(buffer,"+");
            ret = PL0_PLUS;
            break;
        }
        case '-':
        {
            strcpy(buffer,"-");
            ret = PL0_MINUS;
            break;
        }
        case '*':
        {
            strcpy(buffer,"*");
            ret = PL0_TIMES;
            break;
        }
        case '/':
        {
            strcpy(buffer,"/");
            ret = PL0_DIV;
            break;
        }
        case '(':
        {
            strcpy(buffer,"(");
            ret = PL0_LPAREN;
            break;
        }
        case ')':
        {
            strcpy(buffer,")");
            ret = PL0_RPAREN;
            break;
        }
        default:
        {
            if(isdigit(c)){
                int i = 0;
                buffer[i++] = c;
                while(isdigit(peek())){
                    c = get_char();
                    buffer[i++] = c;
                }
                buffer[i] = 0;

                ret = PL0_NUMBER;
            } else if(isalpha(c)){
                int i = 0;
                buffer[i++] = c;
                char nc = peek(); 
                while(isalpha(nc) || nc == '_' || isdigit(nc)){
                    c = get_char();
                    buffer[i++] = c;

                    nc = peek();
                }
                buffer[i] = 0;

                if(!strcmp(buffer,"const")){
                    return PL0_CONST;
                } else if(!strcmp(buffer,"var")){
                    return PL0_VAR;
                } else if(!strcmp(buffer,"procedure")){
                    return PL0_PROCEDURE;
                } else if(!strcmp(buffer,"call")){
                    return PL0_CALL;
                } else if(!strcmp(buffer,"begin")){
                    return PL0_BEGIN;
                } else if(!strcmp(buffer,"end")){
                    return PL0_END;
                } else if(!strcmp(buffer,"if")){
                    return PL0_IF;
                } else if(!strcmp(buffer,"then")){
                    return PL0_THEN;
                } else if(!strcmp(buffer,"while")){
                    return PL0_WHILE;
                } else if(!strcmp(buffer,"do")){
                    return PL0_DO;
                } else if(!strcmp(buffer,"odd")){
                    return PL0_ODD;
                }

                ret = PL0_ID;
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

const char* lexer::get_sym(){
    return buffer;
}
