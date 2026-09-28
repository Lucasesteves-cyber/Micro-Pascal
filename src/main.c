#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "lexer.h"
#include "parser.h"

FILE *src = NULL;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo.pas>\n", argv[0]);
        return 1;
    }

    src = fopen(argv[1], "r");
    if (!src) {
        printf("Erro ao abrir o arquivo: %s\n", argv[1]);
        return 1;
    }

    parse_program();

    printf("Compilacao concluida com sucesso!\n");
    fclose(src);
    return 0;
}