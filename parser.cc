#include <assert.h>
#include <stdlib.h>
#include <string>
#include "parser.h"

using std::string;

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

    string s;
    match_next("program",PL0_PERIOD,s);
}

ast* parser::block(){
    string s;
    PL0_token token = lex->get_token(s);

    ast* ret = new ast(AST_BLOCK);
    if(token == PL0_CONST){
        ast* csub = new ast(AST_CONST);
        ret->add_child(csub);

        match_next("block",PL0_ID,s);
        csub->add_child(new ast(AST_TERMINAL,PL0_ID,s.c_str()));
        match_next("block",PL0_EQ,s);
        match_next("block",PL0_NUMBER,s);
        csub->add_child(new ast(AST_TERMINAL,PL0_NUMBER,s.c_str()));

        token = lex->get_token(s);
        while(token == PL0_COMMA){
            match_next("block",PL0_ID,s);
            csub->add_child(new ast(AST_TERMINAL,PL0_ID,s.c_str()));
            match_next("block",PL0_EQ,s);
            match_next("block",PL0_NUMBER,s);
            csub->add_child(new ast(AST_TERMINAL,PL0_NUMBER,s.c_str()));

            token = lex->get_token(s);
        }
        match("block",token,PL0_SEMICOLON);

        token = lex->get_token(s);
    }

    if(token == PL0_VAR){
        ast* vsub = new ast(AST_VAR);
        vsub->add_child(vsub);

        match_next("block",PL0_ID,s);
        vsub->add_child(new ast(AST_TERMINAL,PL0_ID,s.c_str()));

        token = lex->get_token(s);
        while(token == PL0_COMMA){
            match_next("block",PL0_ID,s);
            vsub->add_child(new ast(AST_TERMINAL,PL0_ID,s.c_str()));

            token = lex->get_token(s);
        }
        match("block",token,PL0_SEMICOLON);

        token = lex->get_token(s);
    }

    while(token == PL0_PROCEDURE){
        ast* psub = new ast(AST_PROCEDURE);
        ret->add_child(psub);

        match_next("block",PL0_ID,s);
        psub->add_child(new ast(AST_TERMINAL,PL0_ID,s.c_str()));
        match_next("block",PL0_SEMICOLON,s);

        psub->add_child(block());
        match_next("block",PL0_SEMICOLON,s);

        token = lex->get_token(s);
    }
    lex->unget_token(token,s);
    ret->add_child(statement());

    token = lex->get_token(s);
    match_block_follow(token);
    lex->unget_token(token,s);

    return ret;
}

void parser::match_block_follow(PL0_token token){
    if(token != PL0_PERIOD && token != PL0_SEMICOLON){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] expected ';' or '.' after block at %d:%d\n",lineno,linepos);

        exit(1);
    }
}

ast* parser::statement(){
    string s;
    PL0_token token = lex->get_token(s);

    ast* ret = new ast(AST_STATEMENT);
    switch(token){ //@TODO: finish
        case PL0_ID:
        {
            ast* assub = new ast(AST_ASSIGN);
            ret->add_child(assub);

            assub->add_child(new ast(AST_TERMINAL,PL0_ID,s.c_str()));
            match_next("statement",PL0_ASSIGN,s);
            assub->add_child(expression());

            break;
        }
        case PL0_CALL:
        {
            ast* csub = new ast(AST_CALL);
            ret->add_child(csub);

            match_next("statement",PL0_ID,s);
            csub->add_child(new ast(AST_TERMINAL,PL0_ID,s.c_str()));
            break;
        }
        case PL0_BEGIN:
        {
            ret->add_child(statement());

            token = lex->get_token(s);
            while(token == PL0_SEMICOLON){
                ret->add_child(statement());

                token = lex->get_token(s);
            }

            match("statement",token,PL0_END);
            break;
        }
        case PL0_IF:
        {
            ast* isub = new ast(AST_IF);
            ret->add_child(isub);

            isub->add_child(condition());
            match_next("statement",PL0_THEN,s);
            isub->add_child(statement());
            break;
        }
        case PL0_WHILE:
        {
            ast* wsub = new ast(AST_WHILE);
            ret->add_child(wsub);

            wsub->add_child(condition());
            match_next("statement",PL0_DO,s);
            wsub->add_child(statement());
            break;
        }
        default:
        {
            lex->unget_token(token,s);
            break;
        }
    }

    token = lex->get_token(s);
    match_statement_follow(token);
    lex->unget_token(token,s);

    return ret;
}

void parser::match_statement_follow(PL0_token token){
    if(token != PL0_SEMICOLON && token != PL0_END && token != PL0_PERIOD && token != PL0_SEMICOLON){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] unexpected token '%s' after 'statement' at %d:%d\n", get_token_map(token),lineno,linepos);

        exit(1);
    }
}

ast* parser::condition(){
    string s;
    PL0_token token = lex->get_token(s);

    ast* ret = new ast(AST_CONDITION);
    if(token == PL0_ODD){
        ast* osub = new ast(AST_ODD);
        ret->add_child(osub);

        osub->add_child(expression());
    } else {
        lex->unget_token(token,s);
        ret->add_child(expression());

        match_next("condition",PL0_COMPARE,s);
        ret->add_child(new ast(AST_TERMINAL,PL0_COMPARE,s.c_str()));

        ret->add_child(expression());
    }

    token = lex->get_token(s);
    match_condition_follow(token);
    lex->unget_token(token,s);

    return ret;
}

void parser::match_condition_follow(PL0_token token){
    if(token != PL0_THEN && token != PL0_DO){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] unexpected token '%s' after 'condition' at %d:%d\n", get_token_map(token),lineno,linepos);

        exit(1);
    }
}


ast* parser::expression(){
    string s;
    PL0_token token = lex->get_token(s);

    ast* cur = NULL;

    //unary operators
    if(token == PL0_PLUS){
        cur = new ast(AST_POSITIVE);
        cur->add_child(term());
    } else if(token == PL0_MINUS){
        cur = new ast(AST_NEGATIVE);
        cur->add_child(term());
    } else {
        lex->unget_token(token,s);
        cur = term();
    }

    token = lex->get_token(s);
    while(token == PL0_PLUS || token == PL0_MINUS){
        ast* ncur = new ast(token == PL0_PLUS ? AST_PLUS : AST_MINUS);
        ncur->add_child(cur);
        ncur->add_child(term());

        cur = ncur;
        token = lex->get_token(s);
    }
    lex->unget_token(token,s);

    match_expression_follow(token);
    return cur;
}

void parser::match_expression_follow(PL0_token token){
    bool test = false;
    test = test || (token == PL0_COMPARE);
    test = test || (token == PL0_RPAREN);
    test = test || (token == PL0_SEMICOLON);
    test = test || (token == PL0_END);
    test = test || (token == PL0_COMMA);
    test = test || (token == PL0_PERIOD);
    test = test || (token == PL0_THEN);
    test = test || (token == PL0_DO);

    if(!test){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] unexpected token '%s' after 'expression' at %d:%d\n", get_token_map(token),lineno,linepos);

        exit(1);
    }
}

ast* parser::term(){
    string s;
    ast* cur = factor();

    PL0_token token = lex->get_token(s);
    while(token == PL0_TIMES || token == PL0_DIV){
        ast* ncur = new ast(token == PL0_TIMES ? AST_TIMES : AST_DIV);
        ncur->add_child(cur);
        ncur->add_child(factor());

        cur = ncur;
        token = lex->get_token(s);
    }
    lex->unget_token(token,s);

    match_term_follow(token);
    return cur;
}

void parser::match_term_follow(PL0_token token){
    bool test = false;
    test = test || (token == PL0_PLUS);
    test = test || (token == PL0_MINUS);
    test = test || (token == PL0_COMPARE);
    test = test || (token == PL0_RPAREN);
    test = test || (token == PL0_SEMICOLON);
    test = test || (token == PL0_END);
    test = test || (token == PL0_COMMA);
    test = test || (token == PL0_PERIOD);
    test = test || (token == PL0_THEN);
    test = test || (token == PL0_DO);

    if(!test){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] unexpected token '%s' after 'term' at %d:%d\n", get_token_map(token),lineno,linepos);

        exit(1);
    }
}

ast* parser::factor(){
    string s;
    PL0_token token = lex->get_token(s);

    if(token == PL0_ID || token == PL0_NUMBER){
        return new ast(AST_TERMINAL,token,s.c_str());
    }

    match("factor",token,PL0_LPAREN);
    ast* ret = expression();
    match_next("factor",PL0_RPAREN,s);

    return ret;
}

void parser::match_factor_follow(PL0_token token){
    bool test = false;
    test = test || (token == PL0_TIMES);
    test = test || (token == PL0_DIV);
    test = test || (token == PL0_PLUS);
    test = test || (token == PL0_MINUS);
    test = test || (token == PL0_COMPARE);
    test = test || (token == PL0_RPAREN);
    test = test || (token == PL0_SEMICOLON);
    test = test || (token == PL0_END);
    test = test || (token == PL0_COMMA);
    test = test || (token == PL0_PERIOD);
    test = test || (token == PL0_THEN);
    test = test || (token == PL0_DO);

    if(!test){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] unexpected token '%s' after 'factor' at %d:%d\n", get_token_map(token),lineno,linepos);

        exit(1);
    }
}


void parser::match(const char* func,PL0_token t,PL0_token r){
    bool test = t == r;
    if(!test){
        if(r == PL0_COMPARE)
            test = t == PL0_EQ;
    }

    if(!test){
        int lineno,linepos;
        lex->get_file_pos(&lineno,&linepos);
        fprintf(stderr,"[parser] invalid token '%s' in %s at line %d:%d, expected '%s'\n",get_token_map(t),func,lineno,linepos,get_token_map(r));

        exit(1);
    }
}

void parser::match_next(const char* func,PL0_token r,string& s){
    PL0_token tok = lex->get_token(s);
    match(func,tok,r);
}

ast* parser::get_root(){
    assert(root);
    return root;
}
