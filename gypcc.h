
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenKind;

typedef enum{
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_NUM,
  ND_EQ,
  ND_NEQ,
  ND_LT,
  ND_LEQ,
} NodeKind;

typedef struct Node Node;
typedef struct Token Token;

struct Token{
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

struct Node{
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

extern Token *token;
extern char *code_head;


extern Token *tokenize(char *);
extern Node *expr();
extern void gen(Node *);
