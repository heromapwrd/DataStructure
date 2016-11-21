#ifndef _AVLTREE_H__
#define _AVLTREE_H__
#include "BST.h"

template<typename T>
class AVLTree:public BST<T>
{
public:
	virtual BinNode<T>* Insert(const T& e);
	virtual bool Remove(const T& e);
protected:
	BinNode<T>* TallerChild(BinNode<T>* pParent);
	bool AvlBalance(BinNode<T>* n);
};

template<typename T>
bool AVLTree<T>::AvlBalance(BinNode<T>* n)
{
	if (!n)
		return true;
	int lHeight = n->_lChild ? n->_lChild->Height() : -1;
	int rHeight = n->_rChild ? n->_rChild->Height() : -1;
	int deltaH = lHeight - rHeight;
	if (-2 < deltaH&&deltaH < 2)
		return true;
	else
		return false;
}

template<typename T>
BinNode<T>* AVLTree<T>::TallerChild(BinNode<T>* pParent)
{
	return (pParent->_lChild->Height() > pParent->_lChild->Height()) ? pParent->_lChild :
		(pParent->_lChild->Height() < pParent->_lChild->Height() ? pParent->_rChild :
		(pParent->IsLChild() ? pParent->_lChild : pParent->_rChild));
}

template<typename T>
bool AVLTree<T>::Remove(const T& e)
{
	BinNode<T>*& x = Search(e);
	if (!x) return false;
	RemoveAt(x, _hot); delete x; _size--;
	for (BinNode<T>* g = _hot; g; g = g->_parent)
	{
		if (!AvlBalance(g))
		{
			BinNode<T>*& pParent = g->FromParent();
			BinNode<T>* succ = RotateAt(TallerChild(TallerChild(g)));
			pParent = succ;
			UpdateDepthBelow(succ);
		}
		UpdateHeight(g);
	}
	return true;
}

template<typename T>
BinNode<T>* AVLTree<T>::Insert(const T& e)
{
	BinNode<T>*& x = Search(e);
	_size++;
	if (!x)
	{
		x = BinNode<T>(e, _hot);
		for (BinNode<T>* g = _hot; g; g = g->_parent)
		{
			if (!AvlBalance(g))
			{
				BinNode<T>*& pParent = g->FromParent();
				BinNode<T>* succ = RotateAt(TallerChild(TallerChild(g)));
				pParent = succ;
				UpdateDepthBelow(succ);
			}
			else
				UpdateHeight(g);
		}
	}
	return x;
}


#endif