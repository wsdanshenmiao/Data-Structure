#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define MALLOC(T) ((T*)malloc(sizeof(T)))

typedef struct Node	//����Ķ���
{
	int m_Data;
	struct Node* m_Next;
}Node;

/*
	Return value:
		����Ϊ��ʱ����true
	Parameter:
		list:�����ͷָ��
*/
bool IsEmpty(const Node* const list)	//�ж������Ƿ�Ϊ��
{
	return list->m_Next == NULL;
}

/*
	Return value:
		Ϊ���һ���ڵ�ʱ����true
	Parameter:
		position:��Ҫ�жϵĽڵ��ָ��
*/
bool IsLast(const Node* const position)	//�жϸýڵ��Ƿ�Ϊ���һ���ڵ�
{
	return position->m_Next == NULL;
}

/*
	Return value:
		���ز���Ԫ�ص�λ��
	Parameter:
		data:��Ҫ���ҵ�����
		list:�����ͷָ��
*/
Node* Find(const int data, const Node* list)	//����ĳ��Ԫ��
{
	Node* position;
	for (position = list->m_Next; position != NULL && position->m_Data!= data; position=position->m_Next);
	return position;
}

/* 
	Return value:
		  ����ָ��ǰ��Ԫ��ָ��
	Parameter:
		data:Ҫ���ҵ�Ԫ��
		list������
*/
Node* FindPrevious(const int data, Node* const list)	//����ĳ��Ԫ�ص�ǰ��Ԫ
{
	Node* position = list;
	for (; position->m_Next != NULL && position->m_Next->m_Data != data; position = position->m_Next);
	return position;
}

/*
	Return value:
		�޷���ֵ
	Parameter:
		data:Ҫɾ����Ԫ��
		list:�����ͷָ��
*/
void Delete(const int data, Node* const list)	//ɾ��ĳ��Ԫ��
{
	Node* previous = FindPrevious(data, list);
	if (!IsLast(previous)) {
		Node* tmp = previous->m_Next;
		previous->m_Next = tmp->m_Next;
		free(tmp);
	}
}

/*
	Return value:
		�޷���ֵ
	 Parameter:
		data:Ҫ��ӵ�Ԫ��
		position:Ԫ�������position֮��
*/
void Insert(const int data, Node* position)	//����ĳ��Ԫ��
{
	assert(position);
	Node* tmp = MALLOC(Node);
	if (tmp == NULL) {
		return;
	}
	tmp->m_Data = data;
	tmp->m_Next = position->m_Next;
	position->m_Next = tmp;
}

/*
	Return value:
		����ĳ���ڵ��е�����
	Parameter:
		position:Ҫ��ѯ�Ľڵ�
*/
int Retrieve(Node* position)	//��ѯ�ڵ��е�����
{

	return position->m_Data;
}

/*
	Return value:
		����ɾ�����ݺ������ͷָ��
	Parameter:
		list:Ҫɾ�����ݵ�����
*/
Node* MakeEmpty(Node* const list)	//�������
{
	Node* position = list->m_Next;
	list->m_Next = NULL;
	while (position) {
		Node* tmp = position->m_Next;
		free(position);
		position = tmp;
	}
	return list;
}

/*
	Return value:
		���ؿ�ָ��
	Parameter:
		list:Ҫɾ��������
*/
Node* DeleteList(Node* list)	//ɾ����������
{
	while (list) {
		Node* tmp = list->m_Next;
		free(list);
		list = tmp;
	}
	return list;
}

int main()
{
	Node* list = MALLOC(Node);
	list->m_Next = NULL;
	IsEmpty(list);
	IsLast(list);
	Insert(3, list);
	Insert(4, Find(3, list));
	Find(3, list);
	Retrieve(list->m_Next);
	Delete(3, list);
	list = MakeEmpty(list);
	Insert(1, list);
	list = DeleteList(list);

	return 0;
}