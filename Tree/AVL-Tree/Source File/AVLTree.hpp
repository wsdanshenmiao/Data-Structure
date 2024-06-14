#ifndef __AVLTREE__HPP__
#define __AVLTREE__HPP__

#include <utility>
#include "allocator.hpp"

namespace DSM {

// ���Ľڵ�
template <typename KeyType>
struct AVLTreeNode
{
	AVLTreeNode* m_Left;
	AVLTreeNode* m_Right;
	int m_Height;
	KeyType m_Key;

	AVLTreeNode() = default;
	AVLTreeNode(const KeyType& key, int height = 0, AVLTreeNode* left = nullptr, AVLTreeNode* right = nullptr);
	AVLTreeNode(const AVLTreeNode& other) = default;
};

template <typename K>
AVLTreeNode<K>::AVLTreeNode(const K& key, int height, AVLTreeNode* left, AVLTreeNode* right)
	:m_Key(key), m_Height(height), m_Left(left), m_Right(right) {}




// ��
template <typename KeyType, typename Compare = std::less<KeyType>>
class AVLTree
{
public:
	using Node = AVLTreeNode<KeyType>;
	using NAllocator = DSM::allocator<AVLTreeNode<KeyType>>;
	using KAllocator = DSM::allocator<KeyType>;

	AVLTree();
	~AVLTree();

	inline Node* Find(const KeyType& key) const;
	inline Node* FindMax(const Node* node) const;
	inline Node* FindMin(const Node* node) const;
	inline std::pair<bool, Node*> Insert(const KeyType& key);

private:
	inline std::pair<bool, Node*> _Insert(const KeyType& key, Node* node, Node* parent);
	inline bool IsRight(const Node* node) const;	// �ж��Ƿ���������
	inline bool IsLeft(const Node* node) const;	// �ж��Ƿ���������
	inline int Height(const Node* node) const;	// ��ȡ�ڵ�߶ȣ���Ϊ���򷵻�-1
	inline Node* LeftSingleRotate(Node* node);	// ����
	inline Node* RightSingleRotate(Node* node);	// ����
	inline Node* LeftDoubleRotate(Node* node);	// ������
	inline Node* RightDoubleRotate(Node* node);	// ������

private:
	Node* m_Root;
	size_t m_Size;
};

template <typename K, typename C>
AVLTree<K, C>::AVLTree()
{
	m_Root = nullptr;
	m_Size = 0;
}

template <typename K, typename C>
AVLTree<K, C>::~AVLTree()
{

}



template <typename K, typename C>
inline AVLTreeNode<K>* 
AVLTree<K, C>::Find(const K& key) const
{
	Node* node = m_Root->m_Left;
	C compare;
	for (; node && node->m_Key != key;) {
		if (compare(key, node->m_Key)) {
			node = node->m_Left;
		}
		else {
			node = node->m_Right;
		}
	}
	return node;
}

template <typename K, typename C>
inline AVLTreeNode<K>* 
AVLTree<K, C>::FindMax(const AVLTreeNode<K>* node) const
{
	if (!node) {
		return node;
	}
	while (node->m_Right) {
		node = node->m_Right;
	}
	return node;
}

template <typename K, typename C>
inline AVLTreeNode<K>* 
AVLTree<K, C>::FindMin(const AVLTreeNode<K>* node) const
{
	if (!node) {
		return node;
	}
	while (node->m_Left) {
		node = node->m_Left;
	}
	return node;
}




template <typename K, typename C>
inline bool AVLTree<K, C>::IsLeft(const AVLTreeNode<K>* node) const
{
	return node->m_Parent->m_Left == node;
}

template <typename K, typename C>
inline bool AVLTree<K, C>::IsRight(const AVLTreeNode<K>* node) const
{
	return node->m_Parent->right == node;
}

template <typename K, typename C>
inline int AVLTree<K, C>::Height(const AVLTreeNode<K>* node) const
{
	return node ? node->m_Height : -1;
}



template <typename K, typename C>
inline AVLTreeNode<K>* AVLTree<K, C>::LeftSingleRotate(Node* node)	// ����
{
	Node* k1 = node->m_Left;
	node->m_Left = k1->m_Right;
	k1->m_Right = node;

	// ��ת����µĽڵ�߶Ȳ��䣬��Ҫ������ת�Ľڵ�
	node->m_Height = std::max(Height(node->m_Left), Height(node->m_Right)) + 1;
	k1->m_Height = std::max(Height(k1->m_Left), Height(k1->m_Right)) + 1;

	return k1;
}
template <typename K, typename C>
inline AVLTreeNode<K>* AVLTree<K, C>::RightSingleRotate(Node* node)	// ����
{
	Node* k1 = node->m_Right;
	node->m_Right = k1->m_Left;
	k1->m_Left = node;

	node->m_Height = std::max(Height(node->m_Left), Height(node->m_Right)) + 1;
	k1->m_Height = std::max(Height(k1->m_Left), Height(k1->m_Right)) + 1;

	return k1;
}
template <typename K, typename C>
inline AVLTreeNode<K>* AVLTree<K, C>::LeftDoubleRotate(Node* node)	// ������
{
	// ���Һ���
	node->m_Left = RightSingleRotate(node->m_Left);
	return LeftSingleRotate(node);
}
template <typename K, typename C>
inline AVLTreeNode<K>* AVLTree<K, C>::RightDoubleRotate(Node* node)	// ������
{
	// �������
	node->m_Right = LeftSingleRotate(node->m_Right);
	return RightSingleRotate(node);
}

template <typename K, typename C>
inline std::pair<bool, AVLTreeNode<K>*>
AVLTree<K, C>::_Insert(const K& key, AVLTreeNode<K>* node, AVLTreeNode<K>* parent)
{
	C compare;
	if (!node) {	//
		NAllocator nallocate;
		node = nallocate.allocate(1);
		nallocate.construct(node, Node(key));
		if (compare(key, parent->m_Key)) {	// ���뵽����
			parent->m_Left = node;
		}
		else {
			parent->m_Right = node;
		}
	}
	else if(key == node->m_Key) {	// ��ֵ�ظ�������ʧ��
		return { false,node };
	}
	else if (compare(key, node->m_Key)) {	// �������
		auto mes = _Insert(key, node->m_Left, node);	// ���߶ȼ�һ
		if (!mes.first) {
			return mes;
		}
		node->m_Left = mes.second;
		if (Height(node->m_Left) - Height(node->m_Right) == 2) {	// ������ƻ�ƽ�⣬��ת
			Node* changeNode = compare(key, node->m_Left->m_Key) ? LeftSingleRotate(node) : LeftDoubleRotate(node);
			m_Root = node == m_Root ? changeNode : m_Root;
			node = changeNode;
		}
	}
	else {	// �����Ҳ�
		auto mes = _Insert(key, node->m_Right, node);
		if (!mes.first) {
			return mes;
		}
		node->m_Right = mes.second;	// �Ҳ�߶ȼ�һ
		if (Height(node->m_Right) - Height(node->m_Left) == 2) {	// ������ƻ�ƽ�⣬��ת
			Node* changeNode = compare(node->m_Right->m_Key, key) ? RightSingleRotate(node) : RightDoubleRotate(node);
			m_Root = node == m_Root ? changeNode : m_Root;
			node = changeNode;
		}
	}
	// ���¸߶� 
	node->m_Height = std::max(Height(node->m_Left), Height(node->m_Right)) + 1;

	return { true, node };
}

template <typename K, typename C>
inline std::pair<bool, AVLTreeNode<K>*> 
AVLTree<K, C>::Insert(const K& key)
{
	if (!m_Root) {	// ��Ϊ���ڵ�
		NAllocator nallocate;
		m_Root = nallocate.allocate(1);
		nallocate.construct(m_Root, Node(key));
		++m_Size;
		return { true,m_Root };
	}

	auto ret = _Insert(key, m_Root, nullptr);
	if (ret.first) {
		++m_Size;
	}
	return ret;
}




}

#endif // !__AVLTREE__HPP__
