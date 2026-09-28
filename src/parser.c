#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexer.h"
#include "parser.h"

static void advance(void) {
    current_token = get_next_token();
}

static void error_parse(const char *msg) {
    printf("Erro sintatico na linha %d: %s (token recebido: '%s')\n", current_token.line, msg, current_token.text);
    exit(1);
}

static void match(TokenType type) {
    if (current_token.type == type) {
        advance();
    } else {
        error_parse("Token inesperado");
    }
}

static void decl_var(void);
static void secao_var(void);
static void bloco(void);
static void expressao(void);
static void comando(void);
static void lista_comandos(void);

static void expressao(void) {
    if (current_token.type == TOKEN_ID || current_token.type == TOKEN_NUM) {
        advance();
    } else if (current_token.type == TOKEN_LPAREN) {
        match(TOKEN_LPAREN);
        expressao();
        match(TOKEN_RPAREN);
    } else {
        error_parse("Expressao invalida");
    }

    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS ||
           current_token.type == TOKEN_MULT || current_token.type == TOKEN_DIV) {
        advance();
        expressao();
    }
}

static void decl_var(void) {
    printf("  [Sintatico] Declaracao de variavel: %s\n", current_token.text);
    match(TOKEN_ID);
    while (current_token.type == TOKEN_COMMA) {
        match(TOKEN_COMMA);
        match(TOKEN_ID);
    }
    match(TOKEN_COLON);
    if (current_token.type == TOKEN_INTEGER) {
        match(TOKEN_INTEGER);
    } else if (current_token.type == TOKEN_BOOLEAN) {
        match(TOKEN_BOOLEAN);
    } else {
        error_parse("Tipo invalido esperado (integer ou boolean)");
    }
    match(TOKEN_SEMI);
    if (current_token.type == TOKEN_ID) {
        decl_var();
    }
}

static void secao_var(void) {
    if (current_token.type == TOKEN_VAR) {
        printf("[Sintatico] Reconhecida secao VAR\n");
        match(TOKEN_VAR);
        decl_var();
    }
}

static void comando(void) {
    if (current_token.type == TOKEN_ID) {
        printf("  [Sintatico] Comando de Atribuicao (%s)\n", current_token.text);
        match(TOKEN_ID);
        match(TOKEN_ASSIGN);
        expressao();
    } else if (current_token.type == TOKEN_IF) {
        printf("  [Sintatico] Comando IF\n");
        match(TOKEN_IF);
        expressao();
        match(TOKEN_THEN);
        comando();
        if (current_token.type == TOKEN_ELSE) {
            match(TOKEN_ELSE);
            comando();
        }
    } else if (current_token.type == TOKEN_WHILE) {
        printf("  [Sintatico] Comando WHILE\n");
        match(TOKEN_WHILE);
        expressao();
        match(TOKEN_DO);
        comando();
    } else if (current_token.type == TOKEN_WRITE) {
        printf("  [Sintatico] Comando WRITE\n");
        match(TOKEN_WRITE);
        match(TOKEN_LPAREN);
        expressao();
        match(TOKEN_RPAREN);
    } else if (current_token.type == TOKEN_READ) {
        printf("  [Sintatico] Comando READ\n");
        match(TOKEN_READ);
        match(TOKEN_LPAREN);
        match(TOKEN_ID);
        match(TOKEN_RPAREN);
    } else if (current_token.type == TOKEN_BEGIN) {
        bloco();
    }
}

static void lista_comandos(void) {
    comando();
    while (current_token.type == TOKEN_SEMI) {
        match(TOKEN_SEMI);
        if (current_token.type != TOKEN_END) {
            comando();
        }
    }
}

static void bloco(void) {
    printf("[Sintatico] Inicio de Bloco (BEGIN)\n");
    match(TOKEN_BEGIN);
    lista_comandos();
    match(TOKEN_END);
    printf("[Sintatico] Fim de Bloco (END)\n");
}

void parse_program(void) {
    advance();
    printf("[Sintatico] Inicio do Programa: %s\n", current_token.text);
    match(TOKEN_PROGRAM);
    match(TOKEN_ID);
    match(TOKEN_SEMI);
    secao_var();
    bloco();
    match(TOKEN_DOT);
}