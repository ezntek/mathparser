#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

int main(void) {
    Token* toks;
    Lexer lexer;
    char input_buf[500];

    lexer = Lexer_new();

    while (1) {
        printf("eval> ");
        fgets(input_buf, sizeof(input_buf), stdin);
        input_buf[strlen(input_buf) - 1] = '\0';

        if (!strcmp(input_buf, "exit"))
            break;

        Lexer_set_data(&lexer, input_buf);
        size_t toks_len = Lexer_tokenize(&lexer, &toks);
        Lexer_print_tokens(toks, toks_len);
        free(toks);
    }

    return 0;
}
