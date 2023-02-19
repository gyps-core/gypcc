#include "gypcc.h"

Token *new_token(TokenKind kind, Token *cur, char *str, int len){
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

void error(Token *token, char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  int pos = token->str - code_head;
  fprintf(stderr,"%s\n", code_head);
  fprintf(stderr, "%*s",pos, " ");
  fprintf(stderr,"^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

bool fwdmatch(char *s, char *t){
  return memcmp(s,t,strlen(t))==0;
}

bool isnondigit(char c){
  return c == '_' || ('a'<=c&&c<'z'||'A'<=c&&c<='Z');
}

int len_id(char *p){
  int len = 0;
  while(isnondigit(*p) || isdigit(*p)){
    p++;
    len++;
  }
  return len;
}

//トークンの解析
bool consume(char *op){
  if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
    return false;
  token = token->next;
  return true;
}

void expect(char op){
  if(token->kind != TK_RESERVED || token->str[0] != op)
    error(token,"not '%c'",op);
  token = token->next;
}

int expect_number(){
  if(token->kind != TK_NUM)
    error(token,"unexpected token");
  int val = token->val;
  token = token->next;
  return val;
}

char *expect_id(){
  if(token->kind != TK_ID)
    error(token,"unexpected token");
  char *str = token->str;
  token = token->next;
  return str;
}

bool at_eof(){
  return token->kind == TK_EOF;
}


Token *tokenize(char *p){
  Token head;
  head.next = NULL;
  Token *cur = &head;
  while(*p){
    if (isspace(*p)){
      p++;
      continue;
    }
    if(fwdmatch(p,"==")||fwdmatch(p,"!=")||fwdmatch(p,"<=")||fwdmatch(p,">=")){
      cur = new_token(TK_RESERVED,cur,p,2);
      p +=2;
      continue;
    }
    if(*p == ';' || *p == '=' || *p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' ||*p == '<'||*p=='>'){
      cur = new_token(TK_RESERVED,cur,p++,1);
      continue;
    }
    if(isdigit(*p)){
      cur = new_token(TK_NUM,cur,p,0);
      char *q = p;
      cur->val=strtol(p,&p,10);
      cur->len = p-q;
      continue;
    }
    if(isnondigit(*p)){
      cur = new_token(TK_ID, cur, p, 0);
      cur->len =len_id(p);
      p+=cur->len;
      continue;
    }
    error(cur,"invailed token");
  }
  new_token(TK_EOF, cur, p, 0);
  return head.next;
}


Node *new_node(NodeKind kind, Node *lhs, Node *rhs){
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val){
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

// find variables.
LVar *find_lvar(){
  for(LVar *var = locals; var; var = var->next){
    if(var->len==token->len&&memcmp(var->name, token->str, var->len)==0)
      return var;
  }
  return NULL;
}

Node *code[100];
void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equal();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

void program(){
  int i = 0;
  while(!at_eof()){
    code[i]=stmt();
    i++;
  }
  code[i] = NULL;
}

Node *stmt(){
  Node *node = expr();
  consume(";");
  return node;
}

Node *expr(){
  return assign();
}

Node *assign(){
  Node *node = equal();
  if(consume("="))
    node = new_node(ND_ASSIGN, node, assign());
  return node;
}

Node *equal(){
  Node *node = relational();
  while(true){
    if(consume("==")){
      node = new_node(ND_EQ, node, relational());
      continue;
    }
    else if(consume("!=")){
      node = new_node(ND_NEQ, node, relational());
      continue;
    }
    return node;
  }
  return node;
}


Node *relational(){
  Node *node = add();
  while(true){
    if(consume("<")){
      node = new_node(ND_LT, node, add());
      continue;
    }
    else if(consume("<=")){
      node = new_node(ND_LEQ, node, add());
      continue;
    }
    else if(consume(">")){
      node = new_node(ND_LT, add(), node);
      continue;
    }
    else if(consume(">=")){
      node = new_node(ND_LEQ, add(), node);
      continue;
    }
    return node;
  }
  return node;
}

Node *add(){
  Node *node = mul();
  while(true){
    if(consume("+")){
      node = new_node(ND_ADD, node, mul());
      continue;
    }
    else if(consume("-")){
      node = new_node(ND_SUB, node, mul());
      continue;
    }
    return node;
  }
  return node;
}

Node *mul(){
  Node *node = unary();
  while(true){
    if(consume("*")){
      node = new_node(ND_MUL, node, unary());
      continue;
    }
    else if(consume("/")){
      node = new_node(ND_DIV, node, unary());
      continue;
    }
    return node;
  }
  return node;
}

Node *unary(){
  if(consume("+"))
    return primary();
  if(consume("-"))
    return new_node(ND_SUB, new_node_num(0),primary());
  return primary();
}

Node *primary(){
  if (consume("(")){
    Node *node = expr();
    consume(")");
    return node;
  }
  if (token->kind == TK_ID){
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LOCAL;

    LVar *lvar =find_lvar();
    if(lvar)
      node->offset = lvar->offset;
    else{
      lvar = calloc(1,sizeof(LVar));
      lvar->next=locals;
      lvar->name=token->str;
      lvar->len=token->len;
      lvar->offset=locals->offset+8;
      node->offset=lvar->offset;
      locals=lvar;
    }
    token = token->next;
    return node;
  }
  return new_node_num(expect_number());
}

