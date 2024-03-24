#pragma once

#include <stddef.h>

#define NULLTOKEN                                                              \
    (Token) { .kind = TOKEN_NULL, .text_len = 0, .text = NULL }
#define EOFTOKEN                                                               \
    (Token) { .kind = TOKEN_EOF, .text = NULL, .text_len = 0 }

#define WHITESPACES "\t\n "
#define SEPS        "()"
#define OPS         "+-/*^%"
#define NUMS        "1234567890"

typedef enum {
    TOKEN_LITERAL = 0,
    TOKEN_OPERATOR = 1,
    TOKEN_SEPARATOR = 2,
    TOKEN_NULL = 3,
    TOKEN_EOF = 4,
} Token_Kind;

typedef struct {
    Token_Kind kind;
    size_t text_len;
    const char* text;
} Token;

typedef struct {
    size_t pos;
    const char* data;
    size_t data_len;
} Lexer;

Lexer Lexer_new(void);
void Lexer_set_data(Lexer* l, const char* newstr);
size_t Lexer_tokenize(Lexer* l, Token** res);
void Lexer_print_tokens(const Token* toks, size_t toks_len);