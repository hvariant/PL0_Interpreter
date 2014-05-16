#include <assert.h>
#include <stdlib.h>
#include "parser.h"

parser::parser(lexer* l){
    init(l);
}
parser::parser(){
    init(NULL);
}

parser::~parser(){ 
    if(root)
        delete root;
}

void parser::init(lexer* l){
    lex = l;
    root = NULL;
}

void parser::parse(){
    assert(!root);

    root = new ast(AST_PROGRAM);
    root->add_child(block());

    match_next(PL0_PERIOD);
}

ast* parser::block(){
    PL0_token token = lex->get_token();

    ast* ret = new ast(AST_BLOCK);
    if(token == PL0_CONST){
        ast* csub = new ast(AST_CONST);
        ret->add_child(csub);

        match_next(PL0_ID);
        csub->add_child(new ast(AST_TERM,PL0_ID,lex->get_sym()));
        match_next(PL0_EQ);
        match_next(PL0_NUMBER);
        csub->add_child(new ast(AST_TERM,PL0_NUMBER,lex->get_sym()));

        token = lex->get_token();
        while(token == PL0_COMMA){
            match_next(PL0_ID);
            csub->add_child(new ast(AST_TERM,PL0_ID,lex->get_sym()));
            match_next(PL0_EQ);
            match_next(PL0_NUMBER);
            csub->add_child(new ast(AST_TERM,PL0_NUMBER,lex->get_sym()));

            token = lex->get_token();
        }
        match(token,PL0_SEMICOLON);

        token = lex->get_token();
    }

    if(token == PL0_VAR){
        ast* vsub = new ast(AST_VAR);
        vsub->add_child(vsub);

        match_next(PL0_ID);
        vsub->add_child(new ast(AST_TERM,PL0_ID,lex->get_sym()));

        token = lex->get_token();
        while(token == PL0_COMMA){
            match_next(PL0_ID);
            vsub->add_child(new ast(AST_TERM,PL0_ID,lex->get_sym()));

            token = lex->get_token();
        }
        match(token,PL0_SEMICOLON);

        token = lex->get_token();
    }

    while(token == PL0_PROCEDURE){
        match_next(PL0_ID);
        ret->add_child(new ast(AST_TERM,PL0_ID,lex->get_sym()));
        match_next(PL0_SEMICOLON);

        ret->add_child(block());
        match_next(PL0_SEMICOLON);

        token = lex->get_token();
    }
    lex->unget_token(token);

    ret->add_child(statement());

    if(token != PL0_PERIOD || token != PL0_SEMICOLON){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] expected ';' or '.' after block at %d:%d\n",lineno,linepos);

        exit(1);
    }

    return ret;
}

ast* parser::statement(){ //@TODO: implement
    return NULL;
}

ast* parser::condition(){ //@TODO: implement
    return NULL;
}

ast* parser::expression(){ //@TODO: implement
    return NULL;
}

ast* parser::term(){ //@TODO: implement
    return NULL;
}

ast* parser::factor(){
    //@TODO: implement
}

void parser::match(PL0_token t,PL0_token r){
    if(t != r){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] invalid token '%s' at line %d:%d, expected '%s'\n",get_token_map(t),lineno,linepos,get_token_map(r));

        exit(1);
    }
}

void parser::match_next(PL0_token r){
    PL0_token tok = lex->get_token();
    match(tok,r);
}

ast* parser::get_root(){
    assert(root);
    return root;
}
