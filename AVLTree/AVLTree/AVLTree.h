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
	BinNode<T>* Connect34(BinNode<T>*a, BinNode<T>*b, BinNode<T>*c,
		BinNode<T>*t0, BinNode<T>*t1, BinNode<T>*t2, BinNode<T>*t3);
	BinNode<T>* RotateAt(BinNode<T>* v);
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
BinNode<T>* AVLTree<T>::RotateAt(BinNode<T>* v)
{
	BinNode<T>* p = v->_parent;
	BinNode<T>* g = p->_parent;
	if (p->IsLChild())
	{
		if (v->IsLChild())
		{
			p->_parent = g->_parent;
			return Connect34(v, p, g, v->_lChild, v->_rChild, p->_rChild, g->_rChild);
		}
		else
		{
			v->_parent = g->_parent;
			return Connect34(p, v, g, p->_lChild, v->_lChild, v->_rChild, g->_rChild);
		}
	}
	else
	{
		if (v->IsLChild())
		{
			v->_parent = g->_parent;
			return Connect34(g, v, p, g->_lChild, v->_lChild, v->_rChild, p->_rChild);
		}
		else
		{
			p->_parent = g->_parent;
			return Connect34(g, p, v, g->_lChild, p->_lChild, v->_lChild, v->_rChild);
		}
	}
}

template<typename T>
BinNode<T>* AVLTree<T>::Connect34(BinNode<T>*a, BinNode<T>*b, BinNode<T>*c, BinNode<T>*t0, BinNode<T>*t1, BinNode<T>*t2, BinNode<T>*t3)
{
	a->_lChild = t0; if (t0)t0->_parent = a;
	a->_rChild = t1; if (t1)t1->_parent = a; UpdateHeight(a);
	c->_lChild = t2; if (t2)t2->_parent = c;
	c->_rChild = t3; if (t3)t3->_parent = c; UpdateHeight(c);
	b->_lChild = a; b->_rChild = c;
	a->_parent = b; c->_parent = b;
	UpdateHeight(b);
	return b;
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