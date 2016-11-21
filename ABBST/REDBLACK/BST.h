#ifndef _BST_H__
#define _BST_H__
#include "BinTree.h"

template<typename T>
class BST:public BinTree<T>
{
public:
	static BinNode<T>*& SearchIn(BinNode<T>*& v, const T& e, BinNode<T>*& hot);
	static BinNode<T>* RemoveAt(BinNode<T>*& v, BinNode<T>*& hot);
	static 
public:
	BST();
public:
	virtual BinNode<T>*& Search(const T& e);
	virtual BinNode<T>* Insert(const T& e);
	virtual bool Remove(const T& e);
protected:
	BinNode<T>* _hot;
};

template<typename T>
BinNode<T>* BST<T>::RemoveAt(BinNode<T>*& v, BinNode<T>*& hot)
{
	if (!v) return NULL;
	BinNode<T>* succ = NULL;
	BinNode<T>* w = v;
	if (!v->HasLChild())
		v = succ = v->_rChild;
	else if (!v->HasRChild())
		v = succ = v->_lChild;
	else
	{
		w = w->Succ_In1();
		swap(w->_data, v->_data);
		BinNode<T>* u = w->_parent;
		((u == v) ? u->_rChild : u->_lChild) = succ = w->_rChild;
	}
	
	hot = w->_parent;
	if (succ) succ->_parent = hot;
	if (w == _root) _root = succ;
	delete w;
	_size--;
	UpdateHeightAbove(hot);
	UpdateDepthBelow(succ);
	return succ;
}

template<typename T>
BinNode<T>*& BST<T>::SearchIn(BinNode<T>*& v, const T& e, BinNode<T>*& hot)
{
	if (!v || e == v->_data) return v;
	hot = v;
	return (e <= v->_data) ? SearchIn(v->_lChild, e, hot) : SearchIn(v->_rChild, e, hot);
}

template<typename T>
bool BST<T>::Remove(const T& e)
{
	BinNode<T>*& v = Search(e);
	if (!v) return false;
	RemoveAt(v, _hot);
	return true;
}

template<typename T>
BinNode<T>* BST<T>::Insert(const T& e)
{
	BinNode<T>* v& = Search(e);
	if (!v)
	{
		v = new BinNode<T>(e, _hot);
		_size++;
		UpdateHeightAbove(_hot);
		UpdateDepthBelow(v);
	}
	return v;
}

template<typename T>
BinNode<T>*& BST<T>::Search(const T& e)
{
	return SearchIn(_root, e, _hot = NULL);
}

template<typename T>
BST<T>::BST() :_hot(NULL)
{

}
#endif
