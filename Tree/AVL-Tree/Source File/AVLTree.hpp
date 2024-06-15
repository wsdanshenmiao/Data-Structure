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
	inline Node* FindMax(Node* node) const;
	inline Node* FindMin(Node* node) const;
	inline std::pair<bool, Node*> Insert(const KeyType& key);
	inline std::pair<bool, Node*> Delete(const KeyType& key);
	inline size_t Size() { return m_Size; }

private:
	inline std::pair<bool, Node*> _Insert(const KeyType& key, Node* node, Node* parent);
	inline std::pair<bool, Node*> _Delete(const KeyType& key, Node* node, Node* parent);

	inline int Height(const Node* node) const;	// ��ȡ�ڵ�߶ȣ���Ϊ���򷵻�-1
	inline bool HaveChild(Node* node) const;

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
AVLTree<K, C>::FindMax(AVLTreeNode<K>* node) const
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
AVLTree<K, C>::FindMin(AVLTreeNode<K>* node) const
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
inline int AVLTree<K, C>::Height(const AVLTreeNode<K>* node) const
{
	return node ? node->m_Height : -1;
}

template <typename K, typename C>
inline bool AVLTree<K, C>::HaveChild(Node* node) const
{
	return node->m_Left && node->m_Right;
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


template <typename K, typename C>
inline std::pair<bool, AVLTreeNode<K>*>
AVLTree<K, C>::_Delete(const K& key, Node* node, Node* parent)
{
	if (!node) {
		return { false,nullptr };
	}

	std::pair<bool, Node*> mes;
	if (key < node->m_Key) {
		mes = _Delete(key, node->m_Left, node);
		node->m_Left = mes.second;
		if (Height(node->m_Right) - Height(node->m_Left) == 2) {
			Node* tmp = node->m_Right;
			Node* changeNode = Height(tmp->m_Right) > Height(tmp->m_Left) ? RightSingleRotate(node) : RightDoubleRotate(node);
			m_Root = node == m_Root ? changeNode : m_Root;
			node = changeNode;
		}
	}
	else if (key > node->m_Key) {
		mes = _Delete(key, node->m_Right, node);
		node->m_Right = mes.second;
		if (Height(node->m_Left) - Height(node->m_Right) == 2) {
			Node* tmp = node->m_Left;
			Node* changeNode = Height(tmp->m_Left) > Height(tmp->m_Right) ? LeftSingleRotate(node) : LeftDoubleRotate(node);
			m_Root = node == m_Root ? changeNode : m_Root;
			node = changeNode;
		}
	}
	else {
		if (node->m_Left && node->m_Right) {
			Node* replace = FindMin(node->m_Right);	// Ѱ������ڵ�
			K data = replace->m_Key;
			mes = _Delete(data, node->m_Right, node);	// ɾ������ڵ�
			node->m_Right = mes.second;
			memcpy(&node->m_Key, &data, sizeof(K));
			if (Height(node->m_Left) - Height(node->m_Right) == 2) {
				Node* tmp = node->m_Left;
				Node* changeNode = Height(tmp->m_Left) > Height(tmp->m_Right) ? LeftSingleRotate(node) : LeftDoubleRotate(node);
				m_Root = node == m_Root ? changeNode : m_Root;
				node = changeNode;
			}
		}
		else {
			Node* changeNode;
			if (parent->m_Left == node) {
				parent->m_Left = node->m_Left ? node->m_Left : node->m_Right;
				changeNode = parent->m_Left;	// ��ֹ�ݹ����ʱ�ı����Ľṹ
			}
			else {
				parent->m_Right = node->m_Right ? node->m_Right : node->m_Left;
				changeNode = parent->m_Right;
			}
			NAllocator nallo;
			KAllocator kallo;
			kallo.destroy(&node->m_Key);
			nallo.destroy(node);
			nallo.deallocate(node, 1);
			return { true,changeNode };
		}
	}
	node->m_Height = std::max(Height(node->m_Left), Height(node->m_Right)) + 1;
	
	return { mes.first,node };
}

template <typename K, typename C>
inline std::pair<bool, AVLTreeNode<K>*>
AVLTree<K, C>::Delete(const K& key)
{
	if (!m_Root) {
		return { false, m_Root };
	}
	
	if (m_Root->m_Key == key && !m_Root->m_Left || !m_Root->m_Right) {
		Node* node = m_Root;
		m_Root = m_Root->m_Left ? m_Root->m_Left : m_Root->m_Right;
		NAllocator nallo;
		KAllocator kallo;
		kallo.destroy(&node->m_Key);
		nallo.destroy(node);
		nallo.deallocate(node, 1);
		m_Size--;
		return { true,nullptr };
	}

	auto mes = _Delete(key, m_Root, nullptr);
	if (mes.first) {
		--m_Size;
	}

	return mes;
}



}

#endif // !__AVLTREE__HPP__
