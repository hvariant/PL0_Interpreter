#include "parser.h"
#include <stdlib.h>

int main(int argc,char* argv[]){
    if(argc != 2){
        fprintf(stderr,"usage: lexer_demo [pl0 source file]\n");

        exit(1);
    }

    lexer lex(argv[1]);
    parser parse(&lex);

    parse.parse();
    ast* root = parse.get_root();

    root->print(stdout);

    return 0;
}
