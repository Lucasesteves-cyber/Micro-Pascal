#ifndef PARSER_H
#define PARSER_H

#include "token.h"

void advance(void);
void match(TokenType type);
void error_syntax(void);

void programa(void);
void secao_var(void);
void decl_var(void);
void tipo(void);
void bloco(void);
void lista_comandos(void);
void comando(void);

#endif