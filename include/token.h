#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_PROGRAM,
    TOKEN_VAR,
    TOKEN_INTEGER,
    TOKEN_BOOLEAN,
    TOKEN_BEGIN,
    TOKEN_END,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_WRITE,
    TOKEN_READ,
    TOKEN_ID,
    TOKEN_NUM,
    TOKEN_ASSIGN,
    TOKEN_SEMI,
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char text[64];
    int line;
} Token;

#endif