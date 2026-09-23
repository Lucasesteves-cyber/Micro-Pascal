#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

static FILE *src;

void lexer_init(FILE *f) {
    src = f;
}

static void error_lex(char c) {
    printf("Erro léxico no caracter [%c]\n", c);
    exit(1);
}

Token get_next_token(void) {
    Token tok;
    int c;

    while ((c = fgetc(src)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;

        if (c == '/') {
            int next = fgetc(src);
            if (next == '/') {
                while ((c = fgetc(src)) != EOF && c != '\n');
                continue;
            } else {
                ungetc(next, src);
                tok.type = TOKEN_DIV_REAL;
                strcpy(tok.lexeme, "/");
                return tok;
            }
        }

        if (isalpha(c) || c == '_' || c == '-') {
            int len = 0;
            tok.lexeme[len++] = c;
            while ((c = fgetc(src)) != EOF && (isalnum(c) || c == '_' || c == '-')) {
                if (len < 255) tok.lexeme[len++] = c;
            }
            ungetc(c, src);
            tok.lexeme[len] = '\0';

            if (strcmp(tok.lexeme, "program") == 0) tok.type = TOKEN_PROGRAM;
            else if (strcmp(tok.lexeme, "var") == 0) tok.type = TOKEN_VAR;
            else if (strcmp(tok.lexeme, "integer") == 0) tok.type = TOKEN_INTEGER;
            else if (strcmp(tok.lexeme, "real") == 0) tok.type = TOKEN_REAL;
            else if (strcmp(tok.lexeme, "char") == 0) tok.type = TOKEN_CHAR;
            else if (strcmp(tok.lexeme, "begin") == 0) tok.type = TOKEN_BEGIN;
            else if (strcmp(tok.lexeme, "end") == 0) tok.type = TOKEN_END;
            else if (strcmp(tok.lexeme, "if") == 0) tok.type = TOKEN_IF;
            else if (strcmp(tok.lexeme, "then") == 0) tok.type = TOKEN_THEN;
            else if (strcmp(tok.lexeme, "else") == 0) tok.type = TOKEN_ELSE;
            else if (strcmp(tok.lexeme, "while") == 0) tok.type = TOKEN_WHILE;
            else if (strcmp(tok.lexeme, "do") == 0) tok.type = TOKEN_DO;
            else if (strcmp(tok.lexeme, "repeat") == 0) tok.type = TOKEN_REPEAT;
            else if (strcmp(tok.lexeme, "until") == 0) tok.type = TOKEN_UNTIL;
            else if (strcmp(tok.lexeme, "write") == 0) tok.type = TOKEN_WRITE;
            else if (strcmp(tok.lexeme, "div") == 0) tok.type = TOKEN_DIV;
            else if (strcmp(tok.lexeme, "and") == 0) tok.type = TOKEN_AND;
            else if (strcmp(tok.lexeme, "or") == 0) tok.type = TOKEN_OR;
            else if (strcmp(tok.lexeme, "not") == 0) tok.type = TOKEN_NOT;
            else tok.type = TOKEN_IDENT;
            return tok;
        }

        if (isdigit(c)) {
            int len = 0;
            tok.lexeme[len++] = c;
            int is_real = 0;
            while ((c = fgetc(src)) != EOF && (isdigit(c) || c == '.')) {
                if (c == '.') {
                    int next = fgetc(src);
                    ungetc(next, src);
                    if (!isdigit(next)) break;
                    is_real = 1;
                }
                if (len < 255) tok.lexeme[len++] = c;
            }
            ungetc(c, src);
            tok.lexeme[len] = '\0';
            tok.type = is_real ? TOKEN_REAL_LIT : TOKEN_INTEIRO_LIT;
            return tok;
        }

        if (c == '\'') {
            int len = 0;
            tok.lexeme[len++] = c;
            c = fgetc(src);
            if (c == '\\') {
                tok.lexeme[len++] = c;
                c = fgetc(src);
            }
            tok.lexeme[len++] = c;
            c = fgetc(src);
            if (c == '\'') {
                tok.lexeme[len++] = c;
                tok.lexeme[len] = '\0';
                tok.type = TOKEN_CHAR_LIT;
                return tok;
            }
            error_lex(c);
        }

        tok.lexeme[0] = c;
        tok.lexeme[1] = '\0';

        switch (c) {
            case '+': tok.type = TOKEN_MAIS; return tok;
            case '-': tok.type = TOKEN_MENOS; return tok;
            case '*': tok.type = TOKEN_MULT; return tok;
            case '=': tok.type = TOKEN_IGUAL; return tok;
            case ';': tok.type = TOKEN_PONTO_VIRGULA; return tok;
            case ',': tok.type = TOKEN_VIRGULA; return tok;
            case '.': tok.type = TOKEN_PONTO; return tok;
            case '(': tok.type = TOKEN_ABRE_PAR; return tok;
            case ')': tok.type = TOKEN_FECHA_PAR; return tok;
            case ':':
                c = fgetc(src);
                if (c == '=') {
                    strcpy(tok.lexeme, ":=");
                    tok.type = TOKEN_ATRIBUICAO;
                } else {
                    ungetc(c, src);
                    tok.type = TOKEN_DOIS_PONTOS;
                }
                return tok;
            case '<':
                c = fgetc(src);
                if (c == '>') {
                    strcpy(tok.lexeme, "<>");
                    tok.type = TOKEN_DIFERENTE;
                } else if (c == '=') {
                    strcpy(tok.lexeme, "<=");
                    tok.type = TOKEN_MENOR_IGUAL;
                } else {
                    ungetc(c, src);
                    tok.type = TOKEN_MENOR;
                }
                return tok;
            case '>':
                c = fgetc(src);
                if (c == '=') {
                    strcpy(tok.lexeme, ">=");
                    tok.type = TOKEN_MAIOR_IGUAL;
                } else {
                    ungetc(c, src);
                    tok.type = TOKEN_MAIOR;
                }
                return tok;
            default:
                error_lex(c);
        }
    }

    tok.type = TOKEN_EOF;
    strcpy(tok.lexeme, "EOF");
    return tok;
}