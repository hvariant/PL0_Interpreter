#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
    if(argc != 2){
        fprintf(stderr,"usage: lexer_demo [pl0 source file]\n");

        exit(1);
    }

    lexer lex(argv[1]);

    PL0_token token = lex.get_token();
    while(token != PL0_EOF){
        printf("%s[%s]\n",lex.get_sym(),get_token_map(token));

        token = lex.get_token();
    }

    return 0;
}
