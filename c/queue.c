/* 循环队列 */

#include <stdio.h>

#define QUEUE_SIZE 5

//定义结构体
struct Queue{
  int front;  //头指针，队非空时指向对头元素
  int rear;   //尾指针，队非空时指向队尾元素的下一位置
  int data[QUEUE_SIZE];
};

//初始化队列
void initQueue(struct Queue *queue) {
  queue->front = 0;
  queue->rear = 0;
}

//判断队列空
int isEmpty(struct Queue *queue) {
  return (queue->front == queue->rear)?1:0;
}

//判断队列满（少用一个元素空间，约定以“队列头指针在队列尾指针的下一位位置”作为队列呈满状态的标志）
int isFull(struct Queue *queue) {
  return ((queue->rear + 1) % QUEUE_SIZE) == queue->front?1:0;
}

//队列的长度
int LenQueue(struct Queue *queue) {
  return (queue->rear - queue->front + QUEUE_SIZE) % QUEUE_SIZE;
}

//入队操作
int enQueue(struct Queue *queue, int element) {
  //判断队列是否已满
  if (isFull(queue)) {
    return 0;
  }
  queue->data[queue->rear] = element;
  queue->rear = (queue->rear + 1) % QUEUE_SIZE;
  return 1;
}

//出队操作
int deQueue(struct Queue *queue, int *element) {
  //判断队列是否为空
  if (isEmpty(queue)) {
    return 0;
  }
  *element = queue->data[queue->front];
  queue->front = (queue->front + 1) % QUEUE_SIZE;
  return 1;
}

//遍历队列
void printQueue(struct Queue *queue) {
  int i;
  if (isEmpty(queue)) {
    printf("空对列\n");
    return;
  }
  printf("当前队列的内容为：\n");
  for (i = queue->front; i < queue->rear; i++) {
    printf("[%d]:%d\n", i, queue->data[i]);
  }
  printf("--END--\n");
}

int main() {
  int i, out_data;
  struct Queue queue;
  //初始化队列
  initQueue(&queue);
  if (isEmpty(&queue)) {
    printf("队列为空队列\n");
  }
  //入队
  for (i = 0; i < 5; i++) {
    if(enQueue(&queue,i) == 0) {
      printf("队列已满\n");
      break;
    }
  }
  //遍历队列
  printQueue(&queue);
  //出队
  if (deQueue(&queue, &out_data)) {
    printf("出队：[%d]\n", out_data);
  }
  printf("\n");
  //遍历队列
  printQueue(&queue);

  return 0;
}
