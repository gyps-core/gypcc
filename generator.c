#include "gypcc.h"

int label_num =2;
//local valriable
void gen_lval(Node *node){
  if(node->kind != ND_LOCAL)
    fprintf(stderr, "value is not a variable.");
  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

void gen(Node *node){
  switch(node->kind){
    case ND_IF:
      gen(node->elms[0]);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      if(node -> elms[2]){
        printf("  je .L%d\n",label_num);
        gen(node->elms[1]);
        printf("  jmp .L%d\n", label_num+1);
        printf(".L%d:\n", label_num);
        gen(node->elms[2]);
        printf(".L%d:\n", label_num+1);
        label_num+=2;
      }
      else{//expression
        gen(node->elms[1]);
        label_num+=1;
      }
      return;
    case ND_WHILE:
      printf(".L%d:\n",label_num);
      gen(node->elms[0]);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .L%d\n", label_num+1);
      gen(node->elms[1]);
      printf("  jmp .L%d\n",label_num);
      printf(".L%d:\n",label_num+1);
      label_num++;
      return;
    case ND_FOR:
      gen(node->elms[0]);
      printf(".L%d:\n",label_num);
      gen(node->elms[1]);
      printf("  pop rax\n");
      printf("  cmp rax, 0\n");
      printf("  je .L%d\n", label_num+1);
      gen(node->elms[3]);
      gen(node->elms[2]);
      printf("  jmp .L%d\n",label_num);
      printf(".L%d:\n",label_num+1);
      label_num++;
      return;
    case ND_ASSIGN://=
      gen_lval(node->lhs);
      gen(node->rhs);
      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  mov [rax], rdi\n");
      printf("  push rdi\n");
      return;
    case ND_LOCAL://a
      printf("  mov rax, rbp\n");
      printf("  sub rax, %d\n", node->offset);
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
      return;
    case ND_NUM:
      printf("  push %d\n", node->val);
      return;
    case ND_RET:
      gen(node->lhs);
      printf("  pop rax\n");
      printf("  mov rsp, rbp\n");
      printf("  pop rbp\n");
      printf("  ret\n");
      return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");
  
  switch(node->kind){
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
      printf("  idiv rdi\n");
      break;
    case ND_EQ:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_NEQ:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LEQ:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
  }
  printf("  push rax\n");
}
