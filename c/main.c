#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Inappropriate number of arguments\n");
    return 1;
  }

  // Tokenization
  user_input = argv[1];
  token = tokenize();
  Node *node = expr();

  // Output the first half of assembly code
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // Generate code traveling the AST down
  gen(node);

  // Load the result on the top of the stack into RAX
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
