#include <stdio.h>

#define STACK_SIZE 20
struct Stack {
  int datas[STACK_SIZE];  //栈数据保存区
  int top;  //栈顶位置标识，空栈时top = -1
};

//输出化栈
void initStack(struct Stack *stack) {
  stack->top = -1;
}

//清空栈
void Empty(struct Stack *stack) {
  stack->top = -1;
}

//判断栈空
int isEmpty(struct Stack *stack) {
  return (stack->top == -1)?1:0;
}

//判断栈满
int isFull(struct Stack *stack) {
  return (stack->top == STACK_SIZE - 1)?1:0;
}

//入栈操作
int push(struct Stack *stack, int value) {
  if (isFull(stack)) {
    return 0;
  }
  stack->datas[++stack->top] = value;
  return 1;
}

//出栈操作
int pop(struct Stack *stack, int *retValue) {
  if (isEmpty(stack)) {
    return 0;
  }
  *retValue = stack->datas[stack->top--];
  return 1;
}

//读栈操作
int getTop(struct Stack *stack, int *retValue) {
  if (isEmpty(stack)) {
    return 0;
  }
  *retValue = stack->datas[stack->top];
  return 1;
}

//遍历栈
void printStack(struct Stack *stack) {
  int i;
  if (isEmpty(stack)) {
    printf("空栈！\n");
    return;
  }
  printf("当前栈的内容：\n");
  for (i = stack->top; i > -1; i--) {
    printf("[%d]:%d \n", i, stack->datas[i]);
  }
  printf("-=END=-\n");
}

//主函数
int main() {
  int i, out_data;
  struct Stack stack;
  //初始化栈
  initStack(&stack);
  //入栈
  for (i = 0; i <= 10 ; i++) {
    push(&stack, i*10);
  }
  //遍历栈
  printStack(&stack);
  //出栈
  for (i= 0; i <= 5; i++) {
    pop(&stack, &out_data);
    printf("[%d], ", out_data);
  }
  printf("\n");
  //遍历栈
  printStack(&stack);
  return 0;
}
