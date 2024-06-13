#ifndef __TREE__HPP__
#define __TREE__HPP__

#include "allocator.hpp"
#include <utility>

namespace DSM {


/*****************************
Class Declaration
*****************************/

// ���Ľڵ�
template <typename KeyType>
struct TreeNode
{
	TreeNode* m_Parent;
	TreeNode* m_Left;
	TreeNode* m_Right;
	KeyType m_Key;

	TreeNode();
	TreeNode(const KeyType& key, TreeNode* parent = nullptr, TreeNode* left = nullptr, TreeNode* right = nullptr);
	TreeNode(KeyType&& key, TreeNode* parent = nullptr, TreeNode* left = nullptr, TreeNode* right = nullptr);
	TreeNode(const TreeNode& other) = default;
	TreeNode& operator=(const TreeNode& other) = default;
};

/*****************************
Class Definition
*****************************/

template <typename K>
TreeNode<K>::TreeNode()
	:m_Parent(nullptr), m_Left(nullptr), m_Right(nullptr) {}

template <typename K>
TreeNode<K>::TreeNode(const K& key, TreeNode* parent, TreeNode* left, TreeNode* right)
	: m_Key(key), m_Parent(parent), m_Left(left), m_Right(right) {}

template <typename K>
TreeNode<K>::TreeNode(K&& key, TreeNode* parent, TreeNode* left, TreeNode* right)
	: m_Key(std::move(key)), m_Parent(parent), m_Left(left), m_Right(right) {}


/*****************************
Class Declaration
*****************************/

// ��
template <typename KeyType, typename Compare = std::less<KeyType>>
class Tree
{
public:
	using TreeNode = TreeNode<KeyType>;

	Tree();
	~Tree();

	inline void MakeEmpty();	// �����
	inline virtual TreeNode* Insert(const KeyType& key);	// ����ڵ�
	inline virtual bool Delete(const KeyType& key);	// ɾ���ڵ�
	inline TreeNode* Find(const KeyType& key) const;	// ����Ԫ��
	inline TreeNode* FindMax(TreeNode* node = m_Head) const;	// Ѱ��ĳ���ڵ���µ����ֵ��Ĭ��Ϊͷ�ڵ㣬Ҳ����������
	inline TreeNode* FindMin(TreeNode* node = m_Head) const;	// Ѱ��ĳ���ڵ���µ���Сֵ��Ĭ��Ϊͷ�ڵ㣬Ҳ����������

protected:
	inline TreeNode* _Insert(const KeyType& key, TreeNode* node, TreeNode* parent);	// ����ĵݹ�ʵ��
	inline std::pair<bool, TreeNode*> UpdataHeadI(const KeyType& key);	// ����ͷ�ڵ㣬�����
	inline bool UpdataHeadD(const KeyType& key);	// ����ͷ�ڵ�,ɾ����
	inline bool IsRight(const TreeNode* node) const;
	inline bool IsLeft(const TreeNode* node) const;

private:
	TreeNode* m_Head;
	size_t m_Size;
};

template <typename K, typename C>
Tree<K, C>::Tree()
{
	DSM::allocator<TreeNode> nAllocate;
	m_Head = nAllocate.allocate(1);
	m_Head->m_Parent = nullptr;
	m_Head->m_Left = nullptr;
	m_Head->m_Right = nullptr;
	m_Size = 0;
}

template <typename K, typename C>
Tree<K, C>::~Tree()
{
	MakeEmpty();	// �����
	DSM::allocator<TreeNode> nAllocate;
	nAllocate.destroy(m_Head);	// �ͷ�ͷ�ڵ�
}



// ����Ԫ��Ԫ��ʱ����ͷ�ڵ�,ͬʱִ�в����ɾ������
template <typename K, typename C>
inline std::pair<bool, TreeNode<K>*> Tree<K, C>::UpdataHeadI(const K& key)
{
	TreeNode* left = m_Head->m_Left, * right = m_Head->m_Right;
	DSM::allocator<TreeNode> nAllo;
	DSM::allocator<K> kAllo;
	C cmp;
	if (cmp(key, left->m_Key)) {	// ����С�Ľڵ㻹С
		TreeNode* newNode = nAllo.allocate(1);
		nAllo.construct(newNode, TreeNode(key, left));
		kAllo.construct(&newNode->m_Key, key);
		left->m_Left = newNode;
		m_Head->m_Left = newNode;
		return { true, newNode };
	}
	else if (key > right->m_Key) {	// �����Ľڵ��
		TreeNode* newNode = nAllo.allocate(1);
		nAllo.construct(newNode, TreeNode(key, right));
		kAllo.construct(&newNode->m_Key, key);
		right->m_Right = newNode;
		m_Head->m_Right = newNode;
		return { true, newNode };
	}
	return { false,nullptr };
}

template <typename K, typename C>
inline TreeNode<K>* Tree<K, C>::_Insert(const K& key, TreeNode* node, TreeNode* parent)
{
	if (!node) {
		DSM::allocator<TreeNode> nAllo;
		DSM::allocator<K> kAllo;
		node = nAllo.allocate(1);
		nAllo.construct(node, TreeNode(key, parent));
		kAllo.construct(&node->m_Key, key);
		if (key < parent->m_Key) {
			parent->m_Left = node;
		}
		else {
			parent->m_Right = node;
		}
	}
	else if (key < node->m_Key) {
		node->m_Left = _Insert(key, node->m_Left, node);
	}
	else if (key > node->m_Key) {
		node->m_Right = _Insert(key, node->m_Right, node);
	}

	return node;
}

template <typename K, typename C>
inline TreeNode<K>* Tree<K, C>::Insert(const K& key)	// ����ڵ�
{
	if (!m_Head->m_Parent) {	// ���޸��ڵ㣬���󲻿����޸��ڵ�
		DSM::allocator<TreeNode> nAllo;
		DSM::allocator<K> kAllo;
		TreeNode* newNode = nAllo.allocate(1);
		nAllo.construct(newNode, TreeNode(key, m_Head));
		kAllo.construct(&newNode->m_Key, key);
		m_Head->m_Parent = m_Head->m_Left = m_Head->m_Right = newNode;
		++m_Size;
		return newNode;
	}

	auto isExtreme = UpdataHeadI(key);
	if (isExtreme.first) {
		++m_Size;
		return isExtreme.second;
	}

	TreeNode* node = _Insert(key, m_Head->m_Parent, m_Head);
	if (node) {
		++m_Size;
	}
	return node;
}



template <typename K, typename C>
inline TreeNode<K>* Tree<K, C>::Find(const K& key) const
{
	TreeNode* node;
	for (node = m_Head->m_Parent; node && node->m_Key != key;) {
		C cmp;
		if (cmp(key, node->m_Key)) {
			node = node->m_Left;
		}
		else {
			node = node->m_Right;
		}
	}

	return node;
}

template <typename K,typename C>
inline TreeNode<K>* Tree<K, C>::FindMax(TreeNode* node) const
{
	while (node->m_Right) {
		node = node->m_Right;
	}
	return node;
}

template <typename K, typename C>
inline TreeNode<K>* Tree<K, C>::FindMin(TreeNode* node) const
{
	while (node->m_Left) {
		node = node->m_Left;
	}
	return node;
}



template <typename K, typename C>
inline bool Tree<K, C>::UpdataHeadD(const K& key)	// ����ͷ�ڵ�
{
	TreeNode* left = m_Head->m_Left, * right = m_Head->m_Right;
	if (left->m_Key != key && right->m_Key != key) {
		return false;
	}

	DSM::allocator<TreeNode> nAllo;
	DSM::allocator<K> kAllo;
	TreeNode* deNode= nullptr;
	if (m_Head->m_Parent->m_Key == key) {	// Ҫɾ��ͷ�ڵ�ʱ
		if (IsLeft(m_Head->m_Parent)) {	// ͷ�ڵ����ָ��ָ����ڵ�
			deNode = left;
			m_Head->m_Left = left->m_Right;
			m_Head->m_Parent = left->m_Right;
			left->m_Right->m_Parent = m_Head;
		}
		else {	// ͷ�ڵ����ָ��ָ����ڵ�
			deNode = right;
			m_Head->m_Right = left->m_Left;
			m_Head->m_Parent = left->m_Left;
			left->m_Left->m_Parent = m_Head;
		}
	}
	else if (left->m_Key == key) {
		m_Head->m_Left = left->m_Parent;
		left->m_Parent->m_Left = nullptr;
		deNode = left;
	}
	else if (right->m_Key == key) {
		m_Head->m_Right = right->m_Parent;
		right->m_Parent->m_Right = nullptr;
		deNode = right;
	}
	kAllo.destroy(&deNode->m_Key);
	nAllo.destroy(deNode);
	nAllo.deallocate(deNode, 1);

	return true;

}

template <typename K, typename C>
inline bool Tree<K, C>::Delete(const K& key)
{
	if (!m_Head->m_Parent) {
		return false;
	}
	if (UpdataHeadD(key)) {	// Ҫɾ����Ԫ��Ϊ������С���Ѵ���ֻ��һ��Ԫ�ص����
		--m_Size;
		return true;
	}

	TreeNode* deNode = Find(key);
	if (!deNode) {
		return false;
	}

	DSM::allocator<TreeNode> nAllo;
	DSM::allocator<K> kAllo;
	TreeNode* parent = deNode->m_Parent;
	if (deNode->m_Left && deNode->m_Right) {	// �������ӽڵ�
		TreeNode* replace = FindMin(deNode->m_Right);
		memcpy(&deNode->m_Key, &replace->m_Key, sizeof(K));
		deNode = replace;
		parent = replace->m_Parent;
	}
	if (IsLeft(deNode)) {
		parent->m_Left = deNode->m_Left;
		if (deNode->m_Left) {
			deNode->m_Left->m_Parent = parent;
		}
	}
	else {
		parent->m_Right = deNode->m_Right;
		if (deNode->m_Right) {
			deNode->m_Right->m_Parent = parent;
		}
	}
	kAllo.destroy(&deNode->m_Key);
	nAllo.destroy(deNode);
	nAllo.deallocate(deNode, 1);
	--m_Size;

	return true;
}


template <typename K, typename C>
inline void Tree<K, C>::MakeEmpty()
{

}



template <typename K, typename C>
inline bool Tree<K, C>::IsLeft(const TreeNode* node) const
{
	return node->m_Parent->m_Left == node;
}

template <typename K, typename C>
inline bool Tree<K, C>::IsRight(const TreeNode* node) const
{
	return node->m_Parent->right == node;
}


}

#endif // !TREE__HPP__
