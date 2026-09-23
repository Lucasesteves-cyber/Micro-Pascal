#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

Token current_token;

void error_syntax(void) {
    printf("Erro de sintaxe no token [%s]\n", current_token.lexeme);
    exit(1);
}

void advance(void) {
    current_token = get_next_token();
}

void match(TokenType type) {
    if (current_token.type == type) {
        advance();
    } else {
        error_syntax();
    }
}

static void atribuicao(void);
static void iteracao(void);
static void decisao(void);
static void escrita(void);
static void expressao(void);
static void expr_or(void);
static void expr_rel(void);
static void expr_add(void);
static void expr_mult(void);
static void expr_basica(void);

void programa(void) {
    match(TOKEN_PROGRAM);
    match(TOKEN_IDENT);
    match(TOKEN_PONTO_VIRGULA);
    secao_var();
    bloco();
    match(TOKEN_PONTO);
}

void secao_var(void) {
    if (current_token.type == TOKEN_VAR) {
        advance();
        while (current_token.type == TOKEN_IDENT) {
            decl_var();
        }
    }
}

void decl_var(void) {
    match(TOKEN_IDENT);
    while (current_token.type == TOKEN_VIRGULA) {
        advance();
        match(TOKEN_IDENT);
    }
    match(TOKEN_DOIS_PONTOS);
    tipo();
    match(TOKEN_PONTO_VIRGULA);
}

void tipo(void) {
    if (current_token.type == TOKEN_INTEGER || current_token.type == TOKEN_REAL || current_token.type == TOKEN_CHAR) {
        advance();
    } else {
        error_syntax();
    }
}

void bloco(void) {
    match(TOKEN_BEGIN);
    lista_comandos();
    match(TOKEN_END);
}

void lista_comandos(void) {
    while (current_token.type == TOKEN_BEGIN || current_token.type == TOKEN_IDENT ||
           current_token.type == TOKEN_WHILE || current_token.type == TOKEN_REPEAT ||
           current_token.type == TOKEN_IF || current_token.type == TOKEN_WRITE) {
        comando();
    }
}

void comando(void) {
    if (current_token.type == TOKEN_BEGIN) {
        bloco();
        match(TOKEN_PONTO_VIRGULA);
    } else if (current_token.type == TOKEN_IDENT) {
        atribuicao();
    } else if (current_token.type == TOKEN_WHILE || current_token.type == TOKEN_REPEAT) {
        iteracao();
    } else if (current_token.type == TOKEN_IF) {
        decisao();
    } else if (current_token.type == TOKEN_WRITE) {
        escrita();
    } else {
        error_syntax();
    }
}

static void atribuicao(void) {
    match(TOKEN_IDENT);
    match(TOKEN_ATRIBUICAO);
    expressao();
    match(TOKEN_PONTO_VIRGULA);
}

static void iteracao(void) {
    if (current_token.type == TOKEN_WHILE) {
        advance();
        expressao();
        match(TOKEN_DO);
        comando();
    } else if (current_token.type == TOKEN_REPEAT) {
        advance();
        comando();
        match(TOKEN_UNTIL);
        expressao();
        match(TOKEN_PONTO_VIRGULA);
    }
}

static void decisao(void) {
    match(TOKEN_IF);
    expressao();
    match(TOKEN_THEN);
    comando();
    if (current_token.type == TOKEN_ELSE) {
        advance();
        comando();
    }
}

static void escrita(void) {
    match(TOKEN_WRITE);
    match(TOKEN_ABRE_PAR);
    expressao();
    match(TOKEN_FECHA_PAR);
    match(TOKEN_PONTO_VIRGULA);
}

static void expressao(void) {
    expr_or();
}

static void expr_or(void) {
    expr_rel();
    while (current_token.type == TOKEN_OR || current_token.type == TOKEN_AND) {
        advance();
        expr_rel();
    }
}

static void expr_rel(void) {
    expr_add();
    while (current_token.type == TOKEN_IGUAL || current_token.type == TOKEN_DIFERENTE ||
           current_token.type == TOKEN_MENOR || current_token.type == TOKEN_MENOR_IGUAL ||
           current_token.type == TOKEN_MAIOR || current_token.type == TOKEN_MAIOR_IGUAL) {
        advance();
        expr_add();
    }
}

static void expr_add(void) {
    expr_mult();
    while (current_token.type == TOKEN_MAIS || current_token.type == TOKEN_MENOS) {
        advance();
        expr_mult();
    }
}

static void expr_mult(void) {
    expr_basica();
    while (current_token.type == TOKEN_MULT || current_token.type == TOKEN_DIV_REAL ||
           current_token.type == TOKEN_DIV) {
        advance();
        expr_basica();
    }
}

static void expr_basica(void) {
    if (current_token.type == TOKEN_ABRE_PAR) {
        advance();
        expressao();
        match(TOKEN_FECHA_PAR);
    } else if (current_token.type == TOKEN_NOT) {
        advance();
        expressao();
    } else if (current_token.type == TOKEN_INTEIRO_LIT || current_token.type == TOKEN_REAL_LIT ||
               current_token.type == TOKEN_CHAR_LIT || current_token.type == TOKEN_IDENT) {
        advance();
    } else {
        error_syntax();
    }
}