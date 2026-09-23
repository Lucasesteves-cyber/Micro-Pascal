#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

void lexer_init(FILE *f);
Token get_next_token(void);

#endif