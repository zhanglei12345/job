#include <stdio.h>
#include <stdlib.h>

//单链表节点
struct Node {
  int data;   //数据区域
  struct Node *next;    //指针区域
};

//链表结构
struct SimLink {
  struct Node *head;  //首节点
  int length;   //链表长度
};


//初始化
void initLink(struct SimLink *link) {
  link->head = NULL;  //初始化首节点为NULL
  link->length = 0;
}

//获取表元
struct Node* getElement(struct SimLink *link, int pos) {
  if(pos < 0 || pos > link->length) {
    printf("该数据节点不存在\n");
    return NULL;
  }
  struct Node *p = link->head;
  int i = 0;
  while (i < pos) {
    p = p->next;
    i++;
  }
  return p;
}

//插入元素
int insert(struct SimLink *link, int pos, int val) {
  if(pos < 0 || pos > link->length) {
    printf("插入数据的位置有误\n");
    return 1; //插入元素失败返回1
  }
  //插入表头位置
  if (pos == 0) {
    struct Node *pNew = (struct Node*)malloc(sizeof(struct Node));
    pNew->data = val;
    pNew->next = link->head;
    link->head = pNew;
  }
  //插入到非表头位置
  else {
    //首先找到插入点的前驱
    struct Node *priv = getElement(link, pos-1);
    struct Node *pNew = (struct Node*)malloc(sizeof(struct Node));
    pNew->data = val;
    pNew->next = priv->next;
    priv->next = pNew;
  }
  link->length++; //更新链表长度
  return 0;
}

//查找数据
struct Node* find(struct SimLink *link, int value) {
  struct Node *cur = link->head;
  //比较过程
  while(cur != NULL) {
    //检查当前节点的值是否匹配
    if (cur->data == value) {
      return cur;
    }
    //如果不匹配，则移动到下一个节点
    else {
      cur = cur->next;
    }
  }
  printf("数据未找到\n" );
  //如果未找到数据，返回NULL
  return NULL;
}

//删除数据
void delete(struct SimLink *link, int pos) {
  /* pos从0开始，所以link->length要-1 */
  if(pos < 0 || pos > link->length-1) {
    printf("该数据节点不存在，删除失败\n");
    return;
  }
  struct Node * del;
  //首节点的情况
  if (pos == 0) {
    del = link->head;
    link->head = del->next;
    free(del);
  }
  //非节点的情况
  else {
    //找到被删除节点的前驱
    struct Node *priv = getElement(link, pos-1);
    del = priv->next;
    priv->next = del->next;
    free(del);
  }
  link->length--;
}

//释放链表,从第一个节点开始删除(必须存在至少一个节点)
void freeLink(struct SimLink *link) {
  struct Node *p = link->head->next;
  while (p) {
    link->head->next = p->next;
    free(p);
    p = link->head->next;
  }
  link->length = 0;
}

//遍历链表
void printLink(struct SimLink *link) {
  struct Node *cur = link->head;
  if (link->length == 0) {
    printf("空表 \n");
    return;
  }
  do {
    printf("[%d], ", cur->data);
    cur = cur->next;
  } while(cur->next != NULL);
  printf("[%d] \n", cur->data);
}


//主函数
int main() {
  //使用链表
  int i;
  struct SimLink link;
  struct Node *find_value;
  initLink(&link);
  //插入数据(0 1 2 3 4)
  for (i = 0; i < 5; i++) {
    insert(&link, i , i);
  }
  printLink(&link);
  /* 查找值为3的节点 */
  find_value = find(&link,3);
  printf("find_value->data = [%d]\n", find_value->data);
  delete(&link, 3);
  printLink(&link);
  printf("*****\n");
  delete(&link, 0);
  printLink(&link);
  freeLink(&link);
  return 0;
}
