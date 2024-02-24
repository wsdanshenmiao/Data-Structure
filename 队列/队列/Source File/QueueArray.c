//��ѭ������ʵ�ֶ���
#include "QueueArray.h"

//��β�Ͷ�ͷѭ��
inline size_t A_Circulation(size_t position, QueueArray* queue)
{
	if (++position == queue->m_Capacity) {
		position = 0;
	}
	return position;
}

//�ж϶����Ƿ�Ϊ��
inline bool A_IsEmpty(QueueArray* const queue)
{
	return queue->m_Size == 0;
}

//�ж϶����Ƿ�Ϊ��
inline bool A_IsFull(QueueArray* const queue)
{
	return queue->m_Size == queue->m_Capacity;
}

//����һ������
QueueArray* A_CreateQueue(const size_t capacity)
{
	QueueArray* queue = (QueueArray*)malloc(sizeof(QueueArray));
	if (!queue) {
		return NULL;
	}
	queue->m_Array = (int*)malloc(sizeof(int) * capacity);
	if (!queue) {
		return NULL;
	}
	queue->m_Capacity = capacity;
	queue->m_Size = 0;
	queue->m_Front = 1;
	queue->m_Rear = 0;
	return queue;
}

//���
void A_Enqueue(int data, QueueArray* queue)
{
	assert(!A_IsFull(queue));
	queue->m_Rear = A_Circulation(queue->m_Rear, queue);
	queue->m_Array[queue->m_Rear] = data;
	queue->m_Size++;
}

//����
void A_Dequeue(QueueArray* queue)
{
	assert(!A_IsEmpty(queue));
	queue->m_Front = A_Circulation(queue->m_Front, queue);
	queue->m_Size--;
}

//���ض�ͷ
int A_Front(QueueArray* queue)
{
	assert(!A_IsEmpty(queue));
	return queue->m_Array[queue->m_Front];
}

//���ض�ͷ������
int A_FrontAndDequeue(QueueArray* queue)
{
	assert(!A_IsEmpty(queue));
	int data = queue->m_Array[queue->m_Front];
	queue->m_Front = A_Circulation(queue->m_Front, queue);
	queue->m_Size--;
	return data;
}

//��ն���
void A_MakeEmpty(QueueArray* queue)
{
	queue->m_Front = 1;
	queue->m_Rear = 0;
	queue->m_Size = 0;
}

//ɾ������
QueueArray* A_DisposeQueue(QueueArray* queue)
{
	free(queue->m_Array);
	free(queue);
	queue = NULL;
	return queue;
}