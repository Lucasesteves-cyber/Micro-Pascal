#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

extern Token current_token;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.pas>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Erro ao abrir o arquivo fonte.\n");
        return 1;
    }

    lexer_init(f);
    advance();
    programa();

    if (current_token.type == TOKEN_EOF) {
        printf("Compilacao concluida com sucesso!\n");
    } else {
        error_syntax();
    }

    fclose(f);
    return 0;
}