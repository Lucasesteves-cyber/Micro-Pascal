#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

extern FILE *src;
extern Token current_token;

void error_lex(char c);
Token get_next_token(void);

#endif