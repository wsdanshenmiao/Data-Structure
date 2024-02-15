//ʹ������ʵ��ջ
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node
{
	int m_Data;
	struct Node* m_Next;
}Node;

bool IsEmpty(Node* stack)	//�ж�ջ�Ƿ�Ϊ��
{
	return stack->m_Next == NULL;
}

Node* CreateStack()	//����һ��ջ
{
	Node* stack = (Node*)malloc(sizeof(Node));
	if (!stack) {
		return NULL;
	}
	stack->m_Next = NULL;
	return stack;
}

void Push(int data, Node* stack)	//��ջ
{
	Node* tmp = (Node*)malloc(sizeof(Node));
	if (!tmp) {
		return;
	}
	tmp->m_Data = data;
	tmp->m_Next = stack->m_Next;
	stack->m_Next = tmp;
}

void Pop(Node* stack)	//��ջ
{
	assert(!IsEmpty(stack));
	Node* tmp = stack->m_Next;
	stack->m_Next = tmp->m_Next;
	free(tmp);
	tmp = NULL;
}

void MakeEmpty(Node* stack)	//���ջ��
{
	assert(stack);
	while (!IsEmpty(stack)) {
		Pop(stack);
	}
}

int Top(Node* stack)	//����ջ��
{
	assert(!IsEmpty(stack));
	return stack->m_Next->m_Data;
}

Node* DisposeStack(Node* stack)
{
	if (!IsEmpty(stack)) {
		MakeEmpty(stack);
	}
	free(stack);
	stack = NULL;
	return stack;
}