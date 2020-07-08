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

// The whole input
char *user_input;

// Error report function
// Takes the same argument as printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Reports an error location and exit
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");  // print pos spaces
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Returns true if the next token is the expected symbol
// Returns false otherwise
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

// Moves on to the next token if the next token is the expected symbol
// Reports error otherwise
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "expected '%c'", op);
  token = token->next;
}

// If the next token is the expected symbol,
// moves on to the next token and return the value 
// Reports error otherwise
int expect_number() {
  if (token->kind != TK_NUM)
    error_at(token->str, "expected a number");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// Generates a new token and connect to 'cur'
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// Returns tokenized input string 'p'
Token *tokenize() {
  char *p = user_input;
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // Skip empty characters
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (strchr("+-*/()", *p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }
    
    error(p, "invalid token");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

// Kinds of AST
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // Integer
} NodeKind;

typedef struct Node Node;

// Node type of AST
struct Node {
  NodeKind kind; // Node kind
  Node *lhs;     // Left hand side
  Node *rhs;     // Right hand side
  int val;       // Used only case of kind == ND_NUM
};

Node *new_node(NodeKind kind) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  return node;
}

Node *new_node_num(int val) {
  Node *node = new_node(ND_NUM);
  node->val = val;
  return node;
}

Node *new_sub_tree(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = new_node(kind);
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *expr();
Node *mul();
Node *unary();
Node *primary();

Node *expr() {
  Node *node = mul();
  for (;;) {
    if (consume('+'))
      node = new_sub_tree(ND_ADD, node, mul());
    else if (consume('-'))
      node = new_sub_tree(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();
  for (;;) {
    if (consume('*'))
      node = new_sub_tree(ND_MUL, node, unary());
    else if (consume('/'))
      node = new_sub_tree(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume('+'))
    return primary();
  if (consume('-')) 
    return new_sub_tree(ND_SUB, new_node_num(0), primary());
  return primary();
}

Node *primary() {
  if (consume('(')) {
    Node *node = expr();
    expect(')');
    return node;
  }
  return new_node_num(expect_number());
}

void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }
  gen(node->lhs);
  gen(node->rhs);
  printf("  pop rdi\n");
  printf("  pop rax\n");
  switch (node->kind) {
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv rax, rdi\n");
      break;
  }
  printf("  push rax\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Inappropriate number of arguments\n");
    return 1;
  }

  // Tokenization
  user_input = argv[1];
  token = tokenize();
  Node *node = expr();

  // Outputs the first half of assembly code
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // Generates code travel the AST down
  gen(node);

  // Loads the result on the top of the stack into RAX
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
