#ifndef _BINNODE_H__
#define _BINNODE_H__
#include <stack>
#include <queue>
using namespace std;

enum RBColor
{
	RB_RED,
	RB_BLACK
};

template<typename T> 
struct BinNode{
public:
	BinNode();
	BinNode(
		const T &e, 
		BinNode<T>* pParent = NULL, 
		BinNode<T>* plChild = NULL, 
		BinNode<T>* prChild = NULL, 
		int h = 0, 
		int d = 0, 
		int l = 1, 
		RBColor color = RB_RED
		);
public:
	
	BinNode<T>* InsertAsLC(const T& e);
	BinNode<T>* InsertAsRC(const T& e);
	BinNode<T>* Succ_In1();
	BinNode<T>* Succ_In2();				//简洁版
	BinNode<T>* Succ_Pre();
	BinNode<T>* Succ_Post();
	BinNode<T>* ToBottom();
	BinNode<T>* Pre_Pre();
	BinNode<T>* Pre_In();
	BinNode<T>* Pre_Post();
	bool UpdateHeight();
	void UpdateDepth();
	int Depth();
	int Height();
	int Size();
	void Swap_R();
	void Swap_I();

	BinNode<T>* Sibling();
	BinNode<T>* Uncle();
	BinNode<T>*& FromParent();

	bool IsRoot();
	bool IsLChild();
	bool IsRChild();
	bool HasParent();
	bool HasLChild();
	bool HasRChild();
	bool HasChild();
	bool HasBothChild();
	bool IsLeaf();

	template <typename VST> void TravLevel(VST& visit);
	template <typename VST> void TravPre(VST& visit);
	template <typename VST> void TravIn(VST& visit);
	template <typename VST> void TravPost(VST& visit);

	bool operator < (const BinNode<T>& node);
	bool operator == (const BinNode<T>& node);
	bool operator > (const BinNode<T>& node);
	bool operator >= (const BinNode<T>& node);
	bool operator <= (const BinNode<T>& node);
	bool operator != (const BinNode<T>& node);
	
public:
	T _data;
	BinNode<T>* _parent;
	BinNode<T>* _lChild;
	BinNode<T>* _rChild;
private:
	int _height;
	int _depth;
	int _npl;
	RBColor _color;
};

template<typename T, typename VST>
void TravPre_R(BinNode<T>* pNode, VST& visit);
template<typename T, typename VST>
void TravPre_I1(BinNode<T>* pNode, VST& visit);		//尾递归
template<typename T, typename VST>
void TravPre_I2(BinNode<T>* pNode, VST& visit);		//找规则stack
template<typename T, typename VST>
void TravPre_I3(BinNode<T>* pNode, VST& visit);		//后继版
template<typename T, typename VST>
void TravIn_R(BinNode<T>* pNode, VST& visit);
template<typename T, typename VST>
void TravIn_I1(BinNode<T>* pNode, VST& visit);		//找规则stack
template<typename T, typename VST>
void TravIn_I2(BinNode<T>* pNode, VST& visit);		//找规则stack变形(逻辑相同)
template<typename T, typename VST>
void TravIn_I3(BinNode<T>* pNode, VST& visit);		//后继版
template<typename T, typename VST>
void TravPost_R(BinNode<T>* pNode, VST& visit);
template<typename T, typename VST>
void TravPost_I1(BinNode<T>* pNode, VST& visit);	//找规则stack
template<typename T, typename VST>
void TravPost_I2(BinNode<T>* pNode, VST& visit);	//后继版
template<typename T, typename VST>
void TravL(BinNode<T>* pNode, VST& visit);

template<typename T,typename VST>
void TravPre_R(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	visit(pNode->data);
	TravPre_R(pNode->_lChild, visit);
	TravPre_R(pNode->_rChild, visit);
}

template<typename T,typename VST>
void TravPre_I1(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	stack<BinNode<T>*> nodes;
	nodes.push(pNode);
	while (!nodes.empty())
	{
		pNode = nodes.pop();
		visit(pNode->data);
		if (pNode->HasLChild()) nodes.push(pNode->_lChild);
		if (pNode->HasRChild()) nodes.push(pNode->_rChild);
	}
}

template<typename T, typename VST>
void TravPre_I2(BinNode<T>* pNode, VST& visit)
{
	stack<BinNode<T>*> s;
	while (true)
	{
		while (pNode)
		{
			visit(pNode->data);
			if (pNode->_rChild)
				s.push(pNode->_rChild);
			pNode = pNode->_lChild;
		}
		
		if (s.empty())
			break;
		pNode = s.pop();
	}
}

template<typename T, typename VST>
void TravPre_I3(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	do 
	{
		visit(pNode->data);
	} while (pNode = pNode->Succ_Pre());
}

template<typename T, typename VST>
void TravIn_R(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	TravPre_R(pNode->_lChild, visit);
	visit(pNode->data);
	TravPre_R(pNode->_rChild, visit);
}

template<typename T, typename VST>
void TravIn_I1(BinNode<T>* pNode, VST& visit)
{
	stack<BinNode<T>*> s;
	while (true)
	{
		while (pNode)
		{
			s.push(pNode);
			pNode = pNode->_lChild;
		}

		if (s.empty())
			break;
		do 
		{
			pNode = s.pop();
			visit(pNode->_data);
		} while ((!pNode->_rChild)&&(!s.empty()));
		pNode = pNode->_rChild;
	}
}

template<typename T, typename VST>
void TravIn_I2(BinNode<T>* pNode, VST& visit)
{
	stack<BinNode<T>*> s;
	while (true)
	{
		if (pNode)
		{
			s.push(pNode);
			pNode = pNode->_lChild;
		}
		else if (!s.empty())
		{
			pNode = s.pop();
			visit(pNode->_data);
			pNode = pNode->_rChild;
		}
		else
			break;
	}
}

template<typename T, typename VST>
void TravIn_I3(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	while (pNode->_lChild)
		pNode = pNode->_lChild;
	do 
	{
		visit(pNode->_data);
	} while (pNode->Succ_In2())
}

template<typename T, typename VST>
void TravPost_R(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	TravPre_R(pNode->_lChild, visit);
	TravPre_R(pNode->_rChild, visit);
	visit(pNode->data);
}

template<typename T, typename VST>
void TravPost_I1(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	stack<BinNode<T>*> s;
	s.push(pNode)
	while (!s.empty())
	{
		while (s.top()->HasChild())
		{
			pNode = s.top();
			if (pNode->_rChild)
				s.push(pNode->_rChild);
			if (pNode->_lChild)
				s.push(pNode->_lChild);
		}
		
		do 
		{
			pNode = s.pop();
			visit(pNode->_data);
		} while (!s.empty() && pNode->_parent == s.top());
	}
}

template<typename T, typename VST>
void TravPost_I2(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	while (pNode->_lChild||pNode->_rChild)
		pNode = pNode->_lChild ? pNode->_lChild : pNode->_rChild;
	do 
	{
		visit(pNode->_data);
	} while (pNode->Succ_Post());
}

template<typename T, typename VST>
void TravL(BinNode<T>* pNode, VST& visit)
{
	if (!pNode)
		return;
	queue<BinNode<T>*> q;
	q.push(pNode);
	while (!q.empty())
	{
		pNode = q.pop();
		visit(pNode->_data);
		if (pNode->_lChild)
			q.push(pNode->_lChild);
		if (pNode->_rChild)
			q.push(pNode->_rChild);
	}
}

template<typename T>
bool BinNode<T>::operator != (const BinNode<T>& node)
{
	return (_data != node._data) ? true : false;
}

template<typename T>
bool BinNode<T>::operator <= (const BinNode<T>& node)
{
	return (_data <= node._data) ? true : false;
}

template<typename T>
bool BinNode<T>::operator >= (const BinNode<T>& node)
{
	return (_data >= node._data) ? true : false;
}

template<typename T>
bool BinNode<T>::operator > (const BinNode<T>& node)
{
	return (_data > node._data) ? true : false;
}

template<typename T>
bool BinNode<T>::operator == (const BinNode<T>& node)
{
	return (_data == node._data) ? true : false;
}

template<typename T>
bool BinNode<T>::operator < (const BinNode<T>& node)
{
	return (_data <= node._data) ? true : false;
}

template <typename T> template <typename VST>
void BinNode<T>::TravPost(VST& visit)
{
	switch (rand() % 3)
	{
	case 0:
		TravPost_R(this, visit);
		break;
	case 1:
		TravPost_I1(this, visit);
		break;
	case 2:
		TravPost_I2(this, visit);
		break;
	default:
		TravPost_R(this, visit);
		break;
	}
}

template <typename T> template <typename VST>
void BinNode<T>::TravIn(VST& visit)
{
	switch (rand() % 4)
	{
	case 0:
		TravIn_R(this, visit);
		break;
	case 1:
		TravIn_I1(this, visit);
		break;
	case 2:
		TravIn_I2(this, visit);
		break;
	case 3:
		TravIn_I3(this, visit);
		break;
	default:
		TravIn_R(this, visit);
		break;
	}
}

template <typename T> template <typename VST>
void BinNode<T>::TravPre(VST& visit)
{
	switch (rand() % 4)
	{
	case 0:
		TravPre_R(this, visit);
		break;
	case 1:
		TravPre_I1(this, visit);
		break;
	case 2:
		TravPre_I2(this, visit);
		break;
	case 3:
		TravPre_I3(this, visit);
		break;
	default:
		TravPre_R(this, visit);
		break;
	}
}

template <typename T> template <typename VST>
void BinNode<T>::TravLevel(VST& visit)
{
	TravL(this, visit);
}

template<typename T>
bool BinNode<T>::IsLeaf()
{
	return !HasChild();
}

template<typename T>
bool BinNode<T>::HasBothChild()
{
	return HasLChild() && HasRChild();
}

template<typename T>
bool BinNode<T>::HasChild()
{
	return HasLChild() || HasRChild();
}

template<typename T>
bool BinNode<T>::HasRChild()
{
	return _rChild ? true : false;
}

template<typename T>
bool BinNode<T>::HasLChild()
{
	return _lChild ? true : false;
}

template<typename T>
bool BinNode<T>::HasParent()
{
	return _parent ? true : false;
}

template<typename T>
bool BinNode<T>::IsRChild()
{
	return HasParent() ? ((_parent->_rChild == this)? true : false) : false;
}

template<typename T>
bool BinNode<T>::IsLChild()
{
	return HasParent() ? ((_parent->_lChild == this) ? true : false) : false;
}

template<typename T>
bool BinNode<T>::IsRoot()
{
	return !HasParent();
}

template<typename T>
BinNode<T>*& BinNode<T>::FromParent()
{
	return IsLChild() ? _parent->_lChild : _parent->_rChild;
}

template<typename T>
BinNode<T>* BinNode<T>::Uncle()
{
	if (IsRoot() || _parent->IsRoot()) return NULL;
	if (_parent->IsLChild())
		return _parent->_parent->_rChild;
	else
		return _parent->_parent->_lChild;
}

template<typename T>
BinNode<T>* BinNode<T>::Sibling()
{
	return IsRoot() ? NULL : (IsLChild() ? _parent->_rChild : _parent->_lChild);
}

template<typename T>
void BinNode<T>::Swap_I()
{
	stack<BinNode<T>*> s;
	while (true)
	{
		while (pNode)
		{
			if (pNode->_lChild)
				s.push(pNode->_lChild);
			BinNode<T>* pLeft = _lChild;
			BinNode<T>* pRight = _rChild;
			_lChild = pRight;
			_rChild = pLeft;
			pNode = pNode->_lChild;
		}

		if (s.empty())
			break;
		pNode = s.pop();
	}
}

template<typename T>
void BinNode<T>::Swap_R()
{
	BinNode<T>* pLeft = _lChild;
	BinNode<T>* pRight = _rChild;
	_lChild = pRight;
	_rChild = pLeft;
	if (_lChild)
		_lChild->Swap_R();
	if (_rChild)
		_rChild->Swap_R();
}

template<typename T>
int BinNode<T>::Size()
{
	int lSize = (_lChild ? _lChild->Size : 0);
	int rSize = (_rChild ? _rChild->Size : 0);
	return 1 + lSize + rSize;
}

template<typename T>
int BinNode<T>::Height()
{
	return _height;
}

template<typename T>
int BinNode<T>::Depth()
{
	return _depth;
}

template<typename T>
void BinNode<T>::UpdateDepth()
{
	if (IsRoot())
		_depth = 0;
	else
		_depth = _parent->_depth + 1;
}


template<typename T>
bool BinNode<T>::UpdateHeight()
{
	int lHeight = (_lChild) ? _lChild->_height : -1;
	int rHeight = (_rChild) ? _rChild->_height : -1;
	int oHeight = _height;
	_height = 1 + max(lHeight, rHeight);
	return oHeight != _height;
}

template<typename T>
BinNode<T>* BinNode<T>::Succ_Pre()
{
	if (HasChild())
	{
		return _lChild ? _lChild : _rChild;
	}

	BinNode<T>* pNode = this;
	while (pNode)
	{
		if (pNode->IsLChild())
		{
			if (!pNode->Sibling())
				pNode = pNode->_parent;
			else
				return pNode->Sibling();
			continue;
		}

		if (pNode->IsRChild())
		{
			pNode = pNode->_parent;
			continue;
		}

		return NULL;
	}
}

template<typename T>
BinNode<T>* BinNode<T>::Succ_In1()
{
	if (IsLeaf())
	{
		if (IsLChild())
			return _parent;
		if (IsRChild())
		{
			BinNode<T>* node = _parent;
			while (true)
			{
				if (node->IsLChild())
					return node->_parent;
				if (node->IsRChild())
					node = node->_parent;
				if (IsRoot())
					return NULL;
			}
		}
		return NULL;
	}

	if (HasRChild())
	{
		BinNode<T>* node = _rChild;
		while (node->_lChild)
			node = node->_lChild;
		return node;
	}

	if (IsChild())
	{
		BinNode<T>* node = _parent;
		if (IsRChild())
		{
			while (node->IsRChild())
				node = node->_parent;
			if (!node->IsChild())
				return NULL;
			else
				node = node->_parent;
		}
		return node;
	}
	return NULL;
}

template<typename T>
BinNode<T>* BinNode<T>::Succ_In2()
{
	BinNode<T>* node = this;
	if (_rChild)
	{
		BinNode<T>* node = _rChild;
		while (node->_lChild)
			node = node->_lChild;
		return node;
	}
	else
	{
		while (node->IsRChild())
			node = node->_parent;
		node = node->_parent;
	}
	return node;
}

template<typename T>
BinNode<T>* BinNode<T>::Succ_Post()
{
	if (IsRoot())
		return NULL;
	BinNode<T>* pNode = this;
	if (pNode->IsLChild() && pNode->Sibling())
	{
		return pNode->Sibling()->ToBottom();
	}
	return pNode->_parent;
}

template<typename T>
BinNode<T>* BinNode<T>::ToBottom()
{
	BinNode<T>* pNode = this;
	while (pNode->HasChild())
		pNode = pNode->_lChild ? pNode->_lChild : pNode->_rChild;
	return pNode;
}

template<typename T>
BinNode<T>* BinNode<T>::Pre_Pre()
{
	if (IsRoot())
		return NULL;
	if (IsRChild() && Sibling())
	{
		BinNode<T>* pNode = Sibling();
		while (pNode->HasChild())
		{
			pNode = pNode->_rChild ? pNode->_rChild : pNode->_lChild;
		}
		return pNode;
	}
	return _parent;
}

template<typename T>
BinNode<T>* BinNode<T>::Pre_In()
{
	if (HasLChild())
	{
		BinNode<T>* pNode = _lChild;
		while (pNode->HasRChild)
			pNode = pNode->_rChild;
		return pNode;
	}
	BinNode<T>* pNode = this;
	while (pNode->IsLChild())
		pNode = pNode->_parent;
	return pNode->_parent;
}

template<typename T>
BinNode<T>* BinNode<T>::Pre_Post()
{
	if (HasChild())
		return _rChild ? _rChild : _lChild;
	BinNode<T>* pNode = this;
	while (pNode->IsLChild()||(pNode->IsRChild()&&!pNode->Sibling()))
		pNode = pNode->_parent;
	pNode = pNode->_parent;
	return pNode ? pNode->_lChild : pNode;
	//return pNode->_parent ? pNode->Sibling() : NULL;
}

template<typename T>
BinNode<T>* BinNode<T>::InsertAsRC(const T& e)
{
	_rChild = new BinNode<T>(e,this);
	_rChild->_depth = _depth + 1;
	return _rChild;
}

template<typename T>
BinNode<T>* BinNode<T>::InsertAsLC(const T& e)
{
	_lChild = new BinNode<T>(e,this);
	_lChild->_depth = _depth + 1;
	return _lChild;
}

template<typename T>
BinNode<T>::BinNode
	(
		const T &e, 
		BinNode<T>* pParent /*= NULL*/, 
		BinNode<T>* plChild /*= NULL*/, 
		BinNode<T>* prChild /*= NULL*/, 
		int h /*= 0*/, 
		int d /*= 0*/, 
		int l /*= 1*/, 
		RBColor color /*= RB_RED */
	):
	_data(e),
	_parent(NULL), 
	_lChild(NULL), 
	_rChild(NULL), 
	_height(0), 
	_depth(0), 
	_npl(1), 
	_color(RB_RED)
{

}

template<typename T>
BinNode<T>::BinNode() :
	_parent(NULL), 
	_lChild(NULL), 
	_rChild(NULL), 
	_height(0), 
	_depth(0), 
	_npl(1), 
	_color(RB_RED)
{

}

#endif