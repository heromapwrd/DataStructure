#ifndef _BINTREE_H__
#define _BINTREE_H__
#include "BinNode.h"

template <typename T>
class BinTree
{
public:
	BinTree();
	~BinTree();
	int Size()const;
	bool Empty()const;
	BinNode<T>* Root()const;
	BinNode<T>* InsertAsRoot(const T& e);
	BinNode<T>* InsertAsLC(BinNode<T>* pNode, const T& e);
	BinNode<T>* InsertAsRC(BinNode<T>* pNode, const T& e);
	BinNode<T>* AttachAsLC(BinNode<T>* pNode, BinTree<T>* &pTree);
	BinNode<T>* AttachAsRC(BinNode<T>* pNode, BinTree<T>* &pTree);
	int Remove(BinNode<T>* pNode);
	int RemoveAt(BinNode<T>* pNode);
	BinTree<T>* Secede(BinNode<T>* pNode);
	BinTree<T>* Swap_R();
	BinTree<T>* Swap_I();

	template<typename VST>
	void TravLevel(VST& visit);
	template<typename VST>
	void TravPre(VST& visit);
	template<typename VST>
	void TravIn(VST& visit);
	template<typename VST>
	void TravPost(VST& visit);

	bool operator == (const BinTree<T>& tree);
	bool operator < (const BinTree<T>& tree);

protected:
	virtual bool UpdateHeight(BinNode<T>* pNode);
	void UpdateHeightAbove(BinNode<T>* pNode);
	int UpdateDepth(BinNode<T>* pNode);
	void UpdateDepthBelow(BinNode<T>* pNode);

protected:
	int _size;
	BinNode<T>* _root;
};

template<typename T>
int BinTree<T>::UpdateDepth(BinNode<T>* pNode)
{
	if (!pNode)
		return -1;
	pNode->UpdateDepth();
	return pNode->Depth();
}

template<typename T>
void BinTree<T>::UpdateDepthBelow(BinNode<T>* pNode)
{
	if (!pNode)
		return;
	pNode->UpdateDepth();
	if (pNode->HasLChild())
		UpdateDepthBelow(pNode->_lChild);
	if (pNode->HasRChild())
		UpdateDepthBelow(pNode->_rChild);
}

template <typename T>
void BinTree<T>::UpdateHeightAbove(BinNode<T>* pNode)
{
	while (pNode)
	{
		if (!UpdateHeight(pNode))
			break;
		pNode = pNode->_parent;
	}
}

template <typename T>
bool BinTree<T>::UpdateHeight(BinNode<T>* pNode)
{
	if (!pNode)
		return false;
	return pNode->UpdateHeight();
}

template <typename T>
bool BinTree<T>::operator<(const BinTree<T>& tree)
{

}

template <typename T>
bool BinTree<T>::operator==(const BinTree<T>& tree)
{

}

template <typename T> template<typename VST>
void BinTree<T>::TravPost(VST& visit)
{
	if (_root)
		_root->TravPost(visit);
}

template <typename T> template<typename VST>
void BinTree<T>::TravIn(VST& visit)
{
	if (_root)
		_root->TravIn(visit);
}

template <typename T> template<typename VST>
void BinTree<T>::TravPre(VST& visit)
{
	if (_root)
		_root->TravPre(visit);
}

template <typename T> template<typename VST>
void BinTree<T>::TravLevel(VST& visit)
{
	if (_root)
		_root->TravLevel(visit);
}

template<typename T>
BinTree<T>* BinTree<T>::Swap_I()
{
	if (_root)
		_root->Swap_I();
	return this;
}

template <typename T>
BinTree<T>* BinTree<T>::Swap_R()
{
	if (_root)
		_root->Swap();
	return *this;
}

template <typename T>
BinTree<T>* BinTree<T>::Secede(BinNode<T>* pNode)
{
	if (!pNode)
		return NULL;
	BinNode<T>* &pParentToThis = _root;
	if (pNode == _root)
		pParentToThis = _root;
	else
		pParentToThis = pNode->FromParent();
	pParentToThis = NULL;
	UpdateHeightAbove(pNode->_parent);
	int size = pNode->Size();
	_size -= size;
	pNode->_parent = NULL;
	BinTree<T>* pTree = new BinTree<T>;
	pTree->_root = pNode;
	UpdateDepthBelow(pNode);
	pTree->_size = size;
	return pTree;
}

template <typename T>
int BinTree<T>::Remove(BinNode<T>* pNode)
{
	BinNode<T>* &pParentToThis = _root;
	if (pNode == _root)
		pParentToThis = _root;
	else
		pParentToThis = pNode->FromParent();
	pParentToThis = NULL;
	UpdateHeightAbove(pNode->_parent);
	int n = RemoveAt(pNode);
	_size -= n;
	return n;
}

template<typename T>
int BinTree<T>::RemoveAt(BinNode<T>* pNode)
{
	if (!pNode)
		return 0;
	int n = 1 + RemoveAt(pNode->_lChild) + RemoveAt(pNode->_rChild);
	delete pNode;
	return n;
}

template <typename T>
BinNode<T>* BinTree<T>::AttachAsRC(BinNode<T>* pNode, BinTree<T>* &pTree)
{
	if (!pNode || !pTree)
		return NULL;
	if (pTree->Empty())
	{
		delete pTree;
		return NULL;
	}
	pNode->_rChild = pTree->_root;
	pTree->_root->_parent = pNode;
	int size = pTree->_size;
	_size += size;
	UpdateHeightAbove(pNode);
	UpdateDepthBelow(pNode->_rChild);
	pTree->_root = NULL;
	pTree->_size = 0;
	delete pTree;
	return pNode->_rChild;
}

template <typename T>
BinNode<T>* BinTree<T>::AttachAsLC(BinNode<T>* pNode, BinTree<T>* &pTree)
{
	
	if (!pNode || !pTree)
		return NULL;
	if (pTree->Empty())
	{
		delete pTree;
		return NULL;
	}
	pNode->_lChild = pTree->_root;
	pTree->_root->_parent = pNode;
	int size = pTree->_size;
	_size += size;
	UpdateHeightAbove(pNode);
	UpdateDepthBelow(pNode->_lChild);
	pTree->_root = NULL;
	pTree->_size = 0;
	return pNode->_lChild;
}

template <typename T>
BinNode<T>* BinTree<T>::InsertAsRC(BinNode<T>* pNode, const T& e)
{
	if (!pNode)
		return NULL;
	_size++;
	BinNode<T>* node = pNode->InsertAsRC(e);
	UpdateHeightAbove(node);
	return node;
}

template <typename T>
BinNode<T>* BinTree<T>::InsertAsLC(BinNode<T>* pNode, const T& e)
{
	if (!pNode)
		return NULL;
	_size++;
	BinNode<T>* node = pNode->InsertAsLC(e);
	UpdateHeightAbove(node);
	return node;
}

template <typename T>
BinNode<T>* BinTree<T>::InsertAsRoot(const T& e)
{
	_size = 1;
	_root = new BinNode<T>(e);
	return _root;
}

template <typename T>
BinNode<T>* BinTree<T>::Root() const
{
	return _root;
}

template <typename T>
bool BinTree<T>::Empty() const
{
	return !_root;
}

template <typename T>
int BinTree<T>::Size() const
{
	return _size;
}

template <typename T>
BinTree<T>::~BinTree()
{
	if (0 < _size)
		Remove(_root);
}

template <typename T>
BinTree<T>::BinTree() :_size(0), _root(NULL)
{

}

#endif