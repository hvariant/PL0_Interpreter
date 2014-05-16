#include "tokens.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char** token_map = NULL;

static void init_token_map(){
    size_t n = PL0_TOKEN_NUM;

    token_map = (const char**)malloc((n+10)*sizeof(char*));

    token_map[PL0_CONST] = "PL0_CONST";
    token_map[PL0_VAR] = "PL0_VAR";
    token_map[PL0_PROCEDURE] = "PL0_PROCEDURE";
    token_map[PL0_CALL] = "PL0_CALL";

    token_map[PL0_ID] = "PL0_ID";
    token_map[PL0_NUMBER] = "PL0_NUMBER";

    token_map[PL0_COMMA] = "PL0_COMMA";
    token_map[PL0_PERIOD] = "PL0_PERIOD";
    token_map[PL0_LPAREN] = "PL0_LPAREN";
    token_map[PL0_RPAREN] = "PL0_RPAREN";
    token_map[PL0_SEMICOLON] = "PL0_SEMICOLON";

    token_map[PL0_ASSIGN] = "PL0_ASSIGN";
    token_map[PL0_PLUS] = "PL0_PLUS";
    token_map[PL0_MINUS] = "PL0_MINUS";
    token_map[PL0_TIMES] = "PL0_TIMES";
    token_map[PL0_DIV] = "PL0_DIV";

    token_map[PL0_COMPARE] = "PL0_COMPARE";
    token_map[PL0_ODD] = "PL0_ODD";

    token_map[PL0_BEGIN] = "PL0_BEGIN";
    token_map[PL0_END] = "PL0_END";
    token_map[PL0_IF] = "PL0_IF";
    token_map[PL0_THEN] = "PL0_THEN";
    token_map[PL0_WHILE] = "PL0_WHILE";
    token_map[PL0_DO] = "PL0_DO";

    token_map[PL0_NONTERM] = "PL0_NONTERM";

    token_map[PL0_EOF] = "PL0_EOF";
    token_map[PL0_EQ] = "PL0_EQ";
}

const char* get_token_map(PL0_token t){
    static int first_call = 1;
    if(first_call){
        first_call = 0;
        init_token_map();
    }

    return token_map[t];
}
