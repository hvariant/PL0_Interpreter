#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
    if(argc != 2){
        fprintf(stderr,"usage: lexer_demo [pl0 source file]\n");

        exit(1);
    }

    lexer lex(argv[1]);

    string s;
    PL0_token token = lex.get_token(s);
    while(token != PL0_EOF){
        printf("%s[%s]\n",s.c_str(),get_token_map(token));

        token = lex.get_token(s);
    }

    return 0;
}
