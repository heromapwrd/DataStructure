#ifndef _SPLAY_H__
#define _SPLAY_H__
#include "BST.h"

template<typename T>
class SplayT:public BST<T>
{
public:

	BinNode<T>* Splay(BinNode<T>* v);
protected:

	virtual BinNode<T>*& Search(const T& e) override
	{
		BinNode<T>* p = BST::SearchIn(e,_hot = NULL);
		_root = Splay(p ? p : _hot);
		return p;
	}

	virtual BinNode<T>* Insert(const T& e) override
	{
		if (!_root){ _size++; return _root = new BinNode<T>(e); }
		if (!Search(e))
		{
			_size++; 
			BinNode<T>* v = new BinNode<T>(e, NULL);
			if (e <= _root->_data)
			{
				AttachAsLC(v, _root->_lChild);
				AttachAsRC(v, _root);
				_root->_lChild = NULL;
			}
			else
			{
				AttachAsRC(v, _root->_rChild);
				AttachAsLC(v, _root);
				_root->_rChild = NULL;
			}
			_root = v;
		}
		UpdateHeightAbove(t);
		UpdateDepthBelow(t);
		return _root;
	}

	virtual bool Remove(const T& e) override
	{
		if (!Search(e))
			return false;
		_size--;
		BinNode<T>* w = _root;
		if (!_root->HasLChild())
		{
			_root = _root->_rChild;
			if (_root) _root->_parent = NULL;
		}
		else if (!_root->HasRChild())
		{
			_root = _root->_lChild;
			if (_root) _root->_parent = NULL;

		}
		else
		{
			_root = _root->_rChild;
			_root->_parent = NULL;
			Search(e);
			AttachAsLC(_root, w->_lChild);
		}
		delete w;
		UpdateHeight(_root);
		UpdateDepthBelow(_root);
		return true;
	}
	void AttachAsLC(BinNode<T>* p, BinNode<T>* lC);
	void AttachAsRC(BinNode<T>* p, BinNode<T>* rC);
};

template<typename T>
void SplayT<T>::AttachAsRC(BinNode<T>* p, BinNode<T>* rC)
{
	p->_rChild = rC;
	if (rC) rC->_parent;
}

template<typename T>
void SplayT<T>::AttachAsLC(BinNode<T>* p, BinNode<T>* lC)
{
	p->_lChild = lC;
	if (lC) lC->_parent = p;
}

template<typename T>
BinNode<T>* SplayT<T>::Splay(BinNode<T>* v)
{
	if (!v) return NULL;
	BinNode<T>* p = NULL;
	BinNode<T>* g = NULL;
	while ((p = v->_parent) && (g = p->_parent))
	{
		BinNode<T>* gg = g->_parent;
		BinNode<T>* t0 = NULL;
		BinNode<T>* t1 = NULL;
		BinNode<T>* t2 = NULL;
		BinNode<T>* t3 = NULL;
		if (p->IsLChild())
		{
			if (v->IsLChild())
			{
				t0 = v->_lChild;
				t1 = v->_rChild;
				t2 = p->_rChild;
				t3 = g->_rChild;
				AttachAsLC(p, t1);
				AttachAsLC(g, t2);
				AttachAsRC(p, g);
				AttachAsRC(v, p);
			}
			else
			{
				t0 = p->_lChild;
				t1 = v->_lChild;
				t2 = v->_rChild;
				t3 = g->_rChild;
				AttachAsRC(p, t1);
				AttachAsLC(g, t2);
				AttachAsLC(v, p);
				AttachAsRC(v, g);
			}
		}
		else
		{
			if (v->IsLChild())
			{
				t0 = g->_lChild;
				t1 = v->_lChild;
				t2 = v->_rChild;
				t3 = p->_rChild;
				AttachAsRC(g, t1);
				AttachAsRC(p, t2);
				AttachAsLC(p, g);
				AttachAsLC(v, p);
			}
			else
			{
				t0 = g->_lChild;
				t1 = p->_lChild;
				t2 = v->_lChild;
				t3 = v->_rChild;
				AttachAsRC(g, t1);
				AttachAsLC(p, t2);
				AttachAsLC(v, g);
				AttachAsRC(v, p);
			}
		}
		v->_parent = gg;
		if (gg) (gg->_lChild == g) ? gg->_lChild : g->_rChild = v;
		UpdateHeightAbove(p);
		UpdateHeightAbove(g);
		UpdateDepthBelow(v);
	}

	if (v->_parent)
	{
		BinNode<T>* p = v->_parent;
		if (v->IsLChild())
		{
			AttachAsLC(p, v->_rChild);
			AttachAsRC(v, p);
		}
		else
		{
			AttachAsRC(p, v->_lChild);
			AttachAsLC(v, p);
		}
		UpdateHeightAbove(p);
		UpdateDepthBelow(v);
	}
	return v;
}

#endif