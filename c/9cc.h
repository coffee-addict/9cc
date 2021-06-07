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
  int len;        // length of token
};

// Token at the current position
extern Token *token;

//// The whole input
extern char *user_input;

// Error report function
void error(char *fmt, ...);
// Report an error location and exit
void error_at(char *loc, char *fmt, ...);

// Return true if the next token is the expected symbol
// Return false otherwise
bool consume(char *op);
// Move on to the next token if the next token is the expected symbol
// Reports error otherwise
void expect(char *op);
// If the next token is the expected symbol,
// move on to the next token and return the value 
// Report error otherwise
int expect_number();

bool at_eof();

// Generate a new token and connect to 'cur'
Token *new_token(TokenKind kind, Token *cur, char *str, int len);

bool startwith(char *p, char *q);

// Return tokenized input string 'p'
Token *tokenize();

// Kinds of AST
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LE,  // <=
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

Node *new_node(NodeKind kind); // add new node without having a number 
Node *new_num(int val); // add new node for a number
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);

// expr       = equality
// equality   = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add        = mul ("+" mul | "-" mul)*
// mul        = unary ("*" unary | "/" unary)*
// unary      = ("+" | "-")? unary
// primary    = num | "(" expr ")"
extern Node *expr();
extern Node *equality();
extern Node *relational();
extern Node *add();
extern Node *mul();
extern Node *unary();
extern Node *primary();

// expr       = equality
// equality   = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add        = mul ("+" mul | "-" mul)*
// mul        = unary ("*" unary | "/" unary)*
// unary      = ("+" | "-")? unary
// primary    = num | "(" expr ")"
Node *expr();
Node *equality();
Node *relational();
Node *add ();
Node *mul();
Node *unary();
Node *primary();

void gen(Node *node);
