#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define LEXER_CURRENT_CHAR l->data[l->pos]

static const size_t STRING_GROWTH_FACTOR = 5;

Lexer Lexer_new(void) {
    return (Lexer){
        .pos = 0,
        .data = NULL,
        .data_len = 0,
    };
}

void Lexer_set_data(Lexer* l, const char* newstr) {
    l->data = newstr;
    l->data_len = strlen(newstr);
    l->pos = 0;
}

static void Lexer_trim_left(Lexer* l) {
    while (strchr(WHITESPACES, LEXER_CURRENT_CHAR) != NULL &&
           l->pos <= l->data_len)
        l->pos++;
}

static Token Lexer_next_token(Lexer* l) {
    Token res;
    char* buf;
    size_t buf_len = 0;
    size_t buf_cap = 5;

    if (l->pos == l->data_len) {
        return EOFTOKEN;
    }

    Lexer_trim_left(l);

    if ((buf = calloc(buf_cap, 1)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (strchr(SEPS, LEXER_CURRENT_CHAR) != NULL) {
        buf[buf_len++] = LEXER_CURRENT_CHAR;
        buf[buf_len] = '\0';
        l->pos++;

        res =
            (Token){.kind = TOKEN_SEPARATOR, .text = buf, .text_len = buf_len};

        return res;
    }

    if (strchr(OPS, LEXER_CURRENT_CHAR) != NULL) {
        buf[buf_len++] = LEXER_CURRENT_CHAR;
        buf[buf_len] = '\0';
        l->pos++;

        res = (Token){
            .kind = TOKEN_OPERATOR,
            .text = buf,
            .text_len = buf_len,
        };

        return res;
    }

    char currch;
    while (strchr(OPS, (currch = LEXER_CURRENT_CHAR)) == NULL &&
           strchr(SEPS, currch) == NULL &&
           strchr(WHITESPACES, currch) == NULL) {
        if (buf_len + 2 > buf_cap) { // buf_len + new char + null term
            buf = realloc((void*)buf, buf_cap * STRING_GROWTH_FACTOR);

            if (buf == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        if (strchr(NUMS, currch) == NULL) {
            l->pos++;
            continue;
        }

        buf[buf_len++] = currch;
        l->pos++;
    }

    if (buf_len == 0) {
        free(buf);
        return NULLTOKEN;
    }

    buf[buf_len] = '\0';

    res = (Token){
        .kind = TOKEN_LITERAL,
        .text_len = buf_len,
        .text = buf,
    };

    return res;
}

size_t Lexer_tokenize(Lexer* l, Token** res) {
    Token* toks;
    Token currtok;
    size_t toks_len = 0;
    size_t toks_cap = 5;

    if ((toks = calloc(toks_cap, sizeof(Token))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    while ((currtok = Lexer_next_token(l)).kind != TOKEN_EOF) {
        if (toks_len + 1 > toks_cap) {
            toks = realloc(toks, sizeof(Token) * (toks_cap *= 5));

            if (toks == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        if (currtok.kind == TOKEN_NULL)
            continue;
        else
            toks[toks_len++] = currtok;
    }

    *res = toks;
    return toks_len;
}

void Lexer_print_tokens(const Token* toks, size_t toks_len) {
    for (size_t i = 0; i < toks_len; i++) {
        char* token_kind_text;

        switch (toks[i].kind) {
            case TOKEN_LITERAL: {
                token_kind_text = "literal";
            } break;
            case TOKEN_OPERATOR: {
                token_kind_text = "operator";
            } break;
            case TOKEN_SEPARATOR: {
                token_kind_text = "separator";
            } break;
            case TOKEN_EOF: {
                token_kind_text = "eof";
            } break;
            case TOKEN_NULL: {
                token_kind_text = "null";
            } break;
        }

        printf("%s(\"%s\")\n", token_kind_text, toks[i].text);
    }
}