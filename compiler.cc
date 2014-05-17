#include "codegen.h"
#include "pl0_vm.h"

int main(int argc,char* argv[]){
    if(argc != 2){
        fprintf(stderr,"usage: compiler [pl0 source file]\n");

        exit(1);
    }

    lexer lex(argv[1]);
    parser parse(&lex);

    parse.parse();
    ast* root = parse.get_root();

    //root->print(stdout);

    codegen gen;
    gen.generate_code(root);

    //gen.print(stdout);

    plzero_vm vm;
    vm.run(gen.get_output());

    return 0;
}
