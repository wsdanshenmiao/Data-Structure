//������ʵ�ֶ���
#include "QueueLinkedList.h"

bool L_IsEmpty(const Queue* const queue)	//�ж������Ƿ�Ϊ��
{
	return queue->m_Rear->m_Next == NULL;
}

Queue* L_CreateQueue()	//��������
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	if (!queue) {
		return NULL;
	}
	queue->m_Front = (Node*)malloc(sizeof(Node));
	if (!queue->m_Rear) {
		return NULL;
	}
	queue->m_Front->m_Next = NULL;
	queue->m_Rear = queue->m_Front;
	queue->m_Size = 0;
	return queue;
}

void L_Enqueue(int data, Queue* queue)	//���
{
	Node* tmp = (Node*)malloc(sizeof(Node));
	if (!tmp) {
		return;
	}
	tmp->m_Data = data;
	tmp->m_Next = queue->m_Rear->m_Next;
	queue->m_Rear->m_Next = tmp;
	queue->m_Rear = tmp;
}

