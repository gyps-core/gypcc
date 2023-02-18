#include "gypcc.h"

char *code_head;
Token *token;

int main(int argc, char **argv){
  if(argc !=2){
    fprintf(stderr, "incorrect number of arguments");
    return 1;
  }
  code_head= argv[1];
  token = tokenize(argv[1]);
  Node *node = expr();

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  gen(node);
  printf("  pop rax\n");

  printf("  ret\n");
  return 0;
}



