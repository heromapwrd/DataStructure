#ifndef _BTREE_H__
#define _BTREE_H__
#include "BTNode.h"

template<typename T>
class BTree
{
public:
	const int Order() const { return _order; }
	const int Size() const { return _size; }
	BTNode<T>*& Root()const { return _root; }
	bool Empty()const { return NULL == _root; }
	BTNode<T>* Search(const T& e);
	bool Insert(const T& e);
	bool Remove(const T& e);
	void Remove(BTNode<T>* n);
public:
	BTree(int order = 3) :_size(0), _order(order), _root(NULL), _hot(NULL)
	{
		_root = new BTNode<T>();
	}
	~BTree() { if (_root) Remove(_root); }
protected:
	void SolveOverflow(BTNode<T>* n);
	void SolveUnderflow(BTNode<T>* n);
	void RemoveAt(BTNode<T>* n);
protected:
	int _size;
	int _order;
	BTNode<T>* _root;
	BTNode<T>* _hot;
};

template<typename T>
void BTree<T>::Remove(BTNode<T>* n)
{
	if (!n) return;
	vector<BTNode<T>*>::iterator it = n->_children->begin();
	for (; it != n->_children->end(); it++)
	{
		if (*it)
			Remove(*it);
	}
	RemoveAt(n);
}

template<typename T>
void BTree<T>::RemoveAt(BTNode<T>* n)
{
	if (!n) return;
	delete n;
}

template<typename T>
void BTree<T>::SolveUnderflow(BTNode<T>* n)
{
	//int limit = _order % 2 + _order / 2 - 2;
	if (!n) return;
	int limit = (m+1)/2 -2;
	if (n->_key->size() > limit)
		return;
	BTNode<T>* pParent = n->_parent;
	if (!pParent)
	{
		if (!n->_key->size() && n->_children[0])
		{
			_root = n->_children[0];
			_root->_parent = NULL;
			n->_children[0] = NULL;
			delete n;
		}
		return;
	}

	int r = 0; while (pParent->_children[r] != n) r++;

	if (r > 0)
	{
		BTNode<T>* lb = pParent->_children[r - 1];
		if (lb->_children->size() > (_order + 1) / 2)
		{
			n->_key->emplace(n->_key->begin(), pParent->_key[r - 1]);
			pParent->_key[r - 1] = lb->_key[lb->_key->size() - 1];
			lb->_key->pop_back();
			n->_children->emplace(n->_children->begin(), lb->_children[lb->_children->size() - 1]);
			lb->_children->pop_back();
			if (n->_children[0]) n->_children[0]->_parent = n;
			return;
		}
	}

	if (r < pParent->_children->size() - 1)
	{
		BTNode<T>* rb = pParent->_children[r + 1];
		if (rb->_children->size()>(_order+1)/2)
		{
			n->_key->push_back(pParent->_key[r]);
			pParent->_key[r] = rb->_key[0];
			rb->_key->erase(rb->_key->begin());
			n->_children->push_back(rb->_children->at(0));
			rb->_children->erase(rb->_children->begin());
			if (n->_children[n->_children->size() - 1])
				n->_children[n->_children->size() - 1]->_parent = n;
			return;
		}
	}

	if (r>0)
	{
		BTNode<T>* lb = pParent->_children[r - 1];
		lb->_key->push_back(pParent->_key[r - 1]);
		int lc = lb->_key->size();
		int rc = n->_key->size();
		for (int i = 0; i < rc; i++)
		{
			lb->_key->push_back(n->_key[i]);
			lb->_children->push_back(n->_children[i])
		}
		lb->_children->push_back(n->_children[rc]);
		for (int i = 0; i <= rc; i++)
		{
			if (lb->_children[lc + i])
				lb->_children[lc + i]->_parent = lb;
		}
		pParent->_key->erase(pParent->_key->begin() + r - 1);
		pParent->_children->erase(pParent->_children->begin() + r);
		delete n;
	}
	else
	{
		BTNode<T>* rb = pParent->_children[r + 1];
		rb->_key->emplace(rb->_key->begin(), pParent->_key[r]);
		int rc = rb->_key->size();
		int lc = n->_key->size();
		rb->_children->emplace(rb->_children->begin(), n->_children[lc]);
		if (rb->_children[0])
			rb->_children[0]->_parent = rb;
		for (int i = rc-1; i >= 0; i--)
		{
			rb->_key->emplace(rb->_key->begin(), n->_key[i]);
			rb->_children->emplace(rb->_children->begin(), n->_children[i]);
			if (rb->_children[0])
				rb->_children[0]->_parent = rb;
		}
		pParent->_key->erase(pParent->_key->begin() + r);
		pParent->_children->erase(pParent->_children->begin() + r);
		delete n;
	}
	SolveUnderflow(pParent);
}

template<typename T>
void BTree<T>::SolveOverflow(BTNode<T>* n)
{
	if (!n || n->_key->size() < _order)
		return;
	int i = 0;
	BTNode<T>* nLeft = NULL;
	BTNode<T>* nRight = NULL;
	vector<T> lKey;
	vector<BTNode<T>*> lChildren;
	for (i = 0; i < n->_key->size()/2; i++)
	{
		lKey->push_back(n->_key[i]);
		lChildren->push_back(n->_children[i]);
	}
	lChildren->push_back(n->_children[i]);
	nLeft = new BTNode<T>(lKey, lChildren);
	for (int i = 0; i < nLeft->_children->size(); i++)
	{
		if (nLeft->_children[i])
			nLeft->_children[i]->_parent = nLeft;
	}

	vector<T> rKey;
	vector<BTNode<T>*> rChildren;
	for (i = n->_key->size() / 2 + 1; i < n->_key->size(); i++)
	{
		rKey->push_back(n->_key[i]);
		rChildren->push_back(n->_children[i]);
	}
	rChildren->push_back(n->_children[i]);
	nRight = new BTNode<T>(rKey, rChildren);
	for (int i = 0; i < nRight->_children->size(); i++)
	{
		if (nRight->_children[i])
			nRight->_children[i]->_parent = nRight;
	}

	T& e = n->_key[n->_key->size() / 2];
	BTNode<T>* pParent = NULL;
	if (!n->_parent)
	{
		pParent = new BTNode<T>(e);
		pParent->_children->[0] = nLeft;
		pParent->_children->[1] = nRight;
		_root = pParent;
	}
	else
	{
		pParent = n->_parent;
		int index = 0;
		pParent->Search(e, index);
		_hot->_children[index] = nLeft;
		_hot->InsertKeyFront(e, index);
		_hot->InsertEdgeBack(nRight, index);
	}
	nRight->_parent = pParent;
	nLeft->_parent = pParent;
	delete n;
	n = pParent;
	SolveOverflow(n);
}

template<typename T>
bool BTree<T>::Remove(const T& e)
{
	BTNode<T>* v = Search(e);
	i(!v) return false;
	int index = 0;
	v->Search(e, index);
	if (v->_children[0] != NULL)
	{
		int iNBranch = index + 1;
		BTNode<T>* next = v->_children[iNBranch];
		iNBranch = 0;
		next->Search(e, iNBranch);
		v->_key[index] = next->_key[iNBranch];
		v = next;
		index = iNBranch;
	}
	v->RemoveKey(index);
	v->RemoveChild(index);
	_size--;
	SolveUnderflow(v);
	return true;
}

template<typename T>
bool BTree<T>::Insert(const T& e)
{
	if (Search(e)) return false;
	int index = 0;
	_hot->Search(e, index);
	if (!_hot->InsertKeyFront(e, index))
		return false;
	if (!_hot->InsertEdgeBack(NULL, index))
	{
		_hot->RemoveKey(index);
		return false;
	}
	_size++;
	SolveOverflow(_hot);
	return true;
}

template<typename T>
BTNode<T>* BTree<T>::Search(const T& e)
{
	BTNode<T>* s = _root;
	_hot = NULL;
	while (s)
	{
		int index = -1;
		if (s->Search(e, index))
			break;
		else
		{
			_hot = s;
			s = s->_children[index];
		}
	}
	return s;
}

#endif