#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
  TK_RESERVED,
  TK_ID,
  TK_NUM,
  TK_EOF,
  TK_RET,
  TK_IF,
  TK_ELSE,
  TK_WHILE,
  TK_FOR,
} TokenKind;

typedef enum{
  ND_ASSIGN,
  ND_LOCAL,
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_NUM,
  ND_EQ,
  ND_NEQ,
  ND_LT,
  ND_LEQ,
  ND_RET,
  ND_IF,
  ND_WHILE,
  ND_FOR,
} NodeKind;

typedef struct Node Node;
typedef struct Token Token;
typedef struct LVar LVar;

struct Token{
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

struct LVar{
  LVar *next;
  char *name;
  int len;
  int offset;
};

struct Node{
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  Node **elms;
  int val;
  int offset;
};

extern Token *token;
extern LVar *locals;
extern char *code_head;
extern Token *tokenize(char *);
extern Node *code[100];
extern void program();
extern void gen(Node *);
