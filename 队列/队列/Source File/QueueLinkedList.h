#ifndef __QUEUELINKEDLIST__H__
#define __QUEUELINKEDLIST__H__

#include <stdbool.h>
#include <stdlib.h>

typedef struct Node
{
	int m_Data;
	struct Node* m_Next;
}Node;
typedef struct Queue
{
	Node* m_Rear;	//��β
	Node* m_Front;	//����
	size_t m_Size;
}Queue;

bool L_IsEmpty(const Queue* const queue);	//�ж������Ƿ�Ϊ��
Queue* L_CreateQueue();	//��������
void L_Enqueue(int data, Queue* queue);	//���


#endif // !__QUEUELINKEDLIST__H__
