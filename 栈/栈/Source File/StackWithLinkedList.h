#ifndef __STRACKWITHLINKEDLIST__H__
#define __STRACKWITHLINKEDLIST__H__

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node Node;
inline bool l_IsEmpty(Node* stack);	//�ж�ջ�Ƿ�Ϊ��
Node* l_CreateStack();	//����һ��ջ
void l_Push(int data, Node* stack);
void l_Pop(Node* stack);
void l_MakeEmpty(Node* stack);
int l_Top(Node* stack);
Node* l_DisposeStack(Node* stack);

#endif // !__STRACKWITHLINKEDLIST__H__

