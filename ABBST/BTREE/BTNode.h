#ifndef _BTNODE_H__
#define _BTNODE_H__
#include <vector>
using namespace std;

template<typename T>
class BTNode
{
public:
	BTNode<T>* _parent;
	vector<T> _key;
	vector<BTNode<T>*> _children;

	BTNode() :_parent(NULL)
	{
		_children->insert(NULL);
	}

	BTNode(const T& e, BTNode<T>* pLChild = NULL, BTNode<T>* pRChild = NULL) :_parent(NULL)
	{
		_key->insert(e);
		_children->insert(pLChild);
		_children->insert(pRChild);
	}

	BTNode(vector<T>& key, vector<BTNode<T>*>* children = NULL) :_parent(NULL)
	{
		vector<T>::iterator k = key.begin();
		while (key != key.end())
		{
			_key->insert(*key);
		}
		
		if (NULL == children)
		{
			for (int i = 0; i <= _key->size(); i++)
			{
				_children->push_back(NULL);
			}
		}
		else
		{
			vector<BTNode<T>*>::iterator c = children->begin();
			for (int i = 0; i <= _key->size(); i++)
			{
				if (c != children->end())
				{
					_children->push_back(*c);
					c++;
				}
				else
				{
					_children->push_back(NULL);
				}
			}
		}
		
	}

	bool Search(const T& e, int &index)
	{
		for (int i = 0; i < _key->size(); i++)
		{
			index = i;
			if (_key[i]>e)
				return false;
			if (_key[i] == e)
				return true;
		}
		index = _key->size();
		return false;
	}

	bool InsertKeyFront(const T& e, int index)
	{
		if (index<0 || index>_key->size())
			return false;
		if (index == _key->size())
			_key->push_back(e);
		else
		{
			_key->emplace(_key->begin() + index, e);
		}
		return true;
	}

	bool InsertEdgeBack(BTNode<T>* edge, int index)
	{
		if (index < 0 || index >= _children->size())
			return false;
		_children->emplace_back(_children->begin() + index, edge);
		return true;
	}

	bool RemoveKey(int index)
	{
		if (index < 0 || index >= _key->size())
			return false;
		_key->erase(_key->begin() + index);
		return true;
	}

	bool RemoveChild(int index)
	{
		if (index < 0 || index >= _children->size())
			return false;
		_children->erase(_children->begin() + index);
		return true;
	}
};

#endif