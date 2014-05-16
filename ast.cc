#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

ast::ast(ast_type typ,PL0_token tok, const char* s){
    init(typ,tok,s);
}

ast::~ast(){
    for(int i=0;i<children.size();i++){
        delete children[i];
    }

    if(sym)
        delete sym;
}

void ast::init(ast_type typ,PL0_token tok,const char* s){
    _type = typ;
    _token = tok;
    if(s){
        sym = strdup(s);
    } else {
        sym = NULL;
    }
}

ast* ast::get_child(int i){
    assert(0 <= i && i < children.size());
    return children[i];
}

void ast::print(FILE* fp){
    //@TODO: implement
}
