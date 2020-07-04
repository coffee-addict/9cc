#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kinds of tokens
typedef enum {
  TK_RESERVED,  // symbol
  TK_NUM,       // integer
  TK_EOF,       // end of input
} TokenKind;

typedef struct Token Token;

// Token type
struct Token {
  TokenKind kind; // kind of token
  Token *next;    // next input token
  int val;        // value if 'kind' == TK_NUM
  char *str;      // string of token
};

// Token at the current position
Token *token;

// Error report function
// Takes the same argument as printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Return true if the next token is the expected symbol
// Return false otherwise
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// Move on to the next token if the next token is the expected symbol
// Report error otherwise
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("Not '%c'", op);
  token = token->next;
}

// If the next token is the expected symbol,
// move on to the next token and return the value 
// Report error otherwise
int expect_number() {
  if (token->kind != TK_NUM)
    error("Not a number");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// Generate a new token and connect to 'cur'
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// Return tokenized input string 'p'
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // Skip empty characters
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }
    
    error("Cannot tokenize");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Inappropriate number of arguments\n");
    return 1;
  }

  // Tokenize
  token = tokenize(argv[1]);

  // Output the first half of assembly code
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // Check if the first part of expression is a number, 
  // then output the first mov order
  printf("  mov rax, %d\n", expect_number());

  // Output assembly code consuming the token sequence
  // in the order of `+ <number>` or `- <number>`
  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
