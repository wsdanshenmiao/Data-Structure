#include <stdbool.h>

typedef struct Node
{
	int m_Data;
	struct Node* m_Next;
}Node;
bool IsEmpty(const Node* const list);	//�ж������Ƿ�Ϊ��
bool IsLast(const Node* const position);	//�жϸýڵ��Ƿ�Ϊ���һ���ڵ�
Node* Find(const int data, const Node* list);	//����ĳ��Ԫ��
void Delete(const int data, Node* const list);	//ɾ��ĳ��Ԫ��
void Insert(const int data, Node* position);	//����ĳ��Ԫ��
int Retrieve(Node* position);	//��ѯ�ڵ��е�����
Node* MakeEmpty(Node* const list);	//�������
Node* DeleteList(Node* list);	//ɾ����������