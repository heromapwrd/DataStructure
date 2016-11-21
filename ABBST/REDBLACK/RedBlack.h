#ifndef _REDBLACK_H__
#define _REDBLACK_H__
#include "BST.h"
#define IsBlack(p) ((!p)||(RB_BLACK == p->_color))
#define IsRed(p) (!IsBlack(p))


template <typename T>
class RedBlack:BST<T>
{
public:
	virtual BinNode<T>* Insert(const T& e);
	virtual bool Remove(const T& e);
	int Height(BinNode<T>* pNode);
protected:
	bool HeightNeedUpdate(BinNode<T>* pNode);
	void SolveDoubleRed(BinNode<T>* x);
	void SolveDoubleBlack(BinNode<T>* x);
	virtual int UpdateHeight(BinNode<T>* x);
};

template <typename T>
bool RedBlack<T>::HeightNeedUpdate(BinNode<T>* pNode)
{
	if (!pNode)
		return false;
	if (Height(pNode->_lChild) == Height(pNode->_rChild))
	{
		if (IsRed(pNode) && Height(pNode) != Height(pNode->_lChild))
			return true;
		if (IsBlack(pNode) && Height(pNode) != (Height(pNode->_lChild) + 1))
			return true;
	}
	else
		return true;
	return false;
}

template <typename T>
int RedBlack<T>::Height(BinNode<T>* pNode)
{
	if (!pNode) return -1;
	return pNode->Height();
}

template <typename T>
int RedBlack<T>::UpdateHeight(BinNode<T>* x)
{
	x->_height = max(Height(x->_lChild), Height(x->_rChild));
	if (IsRed(x))
		return x->_height;
	else
		return x->_height++;
}

template <typename T>
void RedBlack<T>::SolveDoubleBlack(BinNode<T>* x)
{
	BinNode<T>* p = x ? x->_parent : _hot; if (!p) return;
	BinNode<T>* s = x->Sibling();
	if (IsBlack(s))
	{
		BinNode<T>* t = NULL;
		if (s->HasLChild() && IsRed(s->_lChild)) t = s->_lChild;
		else if (s->HasRChild() && IsRed(s->_rChild)) t = s->_rChild;
		if (t)
		{
			RBColor oldColor = p->_color;
			BinNode<T>*& pFromParent = p->FromParent();
			BinNode<T>* b = RotateAt(t);
			if (b->HasLChild()) b->_lChild->_color = RB_BLACK; UpdateHeight(b->_lChild);
			if (b->HasRChild()) b->_rChild->_color = RB_BLACK; UpdateHeight(b->_rChild);
			b->_color = oldColor; UpdateHeight(b);
		}
		else
		{
			s->_color = RB_RED; s->_height--;
			if (IsRed(p))
			{
				p->_color = RB_BLACK;
			}
			else
			{
				p->_height--;
				SolveDoubleBlack(p);
			}
		}
	}
	else
	{
		s->_color = RB_BLACK; p->_color = RB_RED;
		BinNode<T>* t = s->IsLChild() ? s->_lChild : s->_rChild;
		_hot = p;
		BinNode<T>*& pFromParent = p->FromParent();
		pFromParent = RotateAt(t);
		SolveDoubleBlack(x);
	}
}

template <typename T>
void RedBlack<T>::SolveDoubleRed(BinNode<T>* x)
{
	if (!x)
		return;
	if (x->IsRoot())
	{
		x->_color = RB_BLACK;
		x->_height++;
		return;
	}
	BinNode<T>* p = x->_parent;
	BinNode<T>* g = p->_parent;
	BinNode<T>* u = x->Uncle();
	if (IsBlack(u))
	{
		if (x->IsLChild() == p->IsLChild())
			p->_color = RB_BLACK;
		else
			x->_color = RB_BLACK;
		g->_color = RB_RED;

		BinNode<T>* gg = g->_parent;
		BinNode<T>*& pFromParent = g->FromParent();
		BinNode<T>* r = RotateAt(x);
		pFromParent = r;
		r->_parent = gg;
	}
	else
	{
		p->_color = RB_BLACK; p->_height++;
		u->_color = RB_BLACK; u->_height++;
		if (!g->IsRoot())
			g->_color = RB_RED;
		SolveDoubleRed(g);
	}
}

template <typename T>
bool RedBlack<T>::Remove(const T& e)
{
	BinNode<T>*& x = Search(e); if (!x) return false;
	BinNode<T>* r = RemoveAt(x, _hot); if (0 >= --_size) return true;
	if (!_hot)
	{
		if (_root)
		{
			_root->_color = RB_BLACK; UpdateHeight(_root); return true;
		}
	}

	if (HeightNeedUpdate(_hot))
		return true;
	if (IsRed(r))
	{
		r->_color = RB_BLACK; r->_height++; return true;
	}
	SolveDoubleBlack(r);
	return true;
}

template <typename T>
BinNode<T>* RedBlack<T>::Insert(const T& e)
{
	BinNode<T>* v = Search(e);
	if (!v)
	{
		v = new BinNode<T>(e, _hot, NULL, NULL);
		_size++;
	}
	SolveDoubleRed(v);
	return v;
}

#endif