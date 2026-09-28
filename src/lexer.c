#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

Token current_token;
int current_line = 1;

void error_lex(char c) {
    printf("Erro lexico na linha %d: caractere invalido '%c'\n", current_line, c);
    exit(1);
}

Token get_next_token(void) {
    int c;
    Token t;
    t.text[0] = '\0';

    while ((c = fgetc(src)) != EOF) {
        if (c == '\n') {
            current_line++;
            continue;
        }
        if (isspace(c)) continue;

        t.line = current_line;

        if (isalpha(c)) {
            int len = 0;
            t.text[len++] = (char)c;
            while ((c = fgetc(src)) != EOF && (isalnum(c) || c == '_')) {
                if (len < 63) t.text[len++] = (char)c;
            }
            if (c != EOF) ungetc(c, src);
            t.text[len] = '\0';

            if (strcasecmp(t.text, "program") == 0) t.type = TOKEN_PROGRAM;
            else if (strcasecmp(t.text, "var") == 0) t.type = TOKEN_VAR;
            else if (strcasecmp(t.text, "integer") == 0) t.type = TOKEN_INTEGER;
            else if (strcasecmp(t.text, "boolean") == 0) t.type = TOKEN_BOOLEAN;
            else if (strcasecmp(t.text, "begin") == 0) t.type = TOKEN_BEGIN;
            else if (strcasecmp(t.text, "end") == 0) t.type = TOKEN_END;
            else if (strcasecmp(t.text, "if") == 0) t.type = TOKEN_IF;
            else if (strcasecmp(t.text, "then") == 0) t.type = TOKEN_THEN;
            else if (strcasecmp(t.text, "else") == 0) t.type = TOKEN_ELSE;
            else if (strcasecmp(t.text, "while") == 0) t.type = TOKEN_WHILE;
            else if (strcasecmp(t.text, "do") == 0) t.type = TOKEN_DO;
            else if (strcasecmp(t.text, "write") == 0) t.type = TOKEN_WRITE;
            else if (strcasecmp(t.text, "read") == 0) t.type = TOKEN_READ;
            else t.type = TOKEN_ID;
            return t;
        }

        if (isdigit(c)) {
            int len = 0;
            t.text[len++] = (char)c;
            while ((c = fgetc(src)) != EOF && isdigit(c)) {
                if (len < 63) t.text[len++] = (char)c;
            }
            if (c != EOF) ungetc(c, src);
            t.text[len] = '\0';
            t.type = TOKEN_NUM;
            return t;
        }

        if (c == ':') {
            int next = fgetc(src);
            if (next == '=') {
                strcpy(t.text, ":=");
                t.type = TOKEN_ASSIGN;
            } else {
                if (next != EOF) ungetc(next, src);
                strcpy(t.text, ":");
                t.type = TOKEN_COLON;
            }
            return t;
        }

        if (c == ';') { strcpy(t.text, ";"); t.type = TOKEN_SEMI; return t; }
        if (c == '.') { strcpy(t.text, "."); t.type = TOKEN_DOT; return t; }
        if (c == ',') { strcpy(t.text, ","); t.type = TOKEN_COMMA; return t; }
        if (c == '(') { strcpy(t.text, "("); t.type = TOKEN_LPAREN; return t; }
        if (c == ')') { strcpy(t.text, ")"); t.type = TOKEN_RPAREN; return t; }
        if (c == '+') { strcpy(t.text, "+"); t.type = TOKEN_PLUS; return t; }
        if (c == '-') { strcpy(t.text, "-"); t.type = TOKEN_MINUS; return t; }
        if (c == '*') { strcpy(t.text, "*"); t.type = TOKEN_MULT; return t; }
        if (c == '/') { strcpy(t.text, "/"); t.type = TOKEN_DIV; return t; }

        error_lex((char)c);
    }

    t.type = TOKEN_EOF;
    t.line = current_line;
    strcpy(t.text, "EOF");
    return t;
}