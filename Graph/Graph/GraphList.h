#ifndef _GRAPHLIST_H__
#define _GRAPHLIST_H__
#include "Graph.h"
#include <vector>
#include <list>
#include<assert.h>
using namespace std;

template<typename Tv,typename Te>
class GraphList:public Graph<Tv,Te>
{
public:
	vector<Vertex<Tv>> V;
	vector<list<Edge<Te>*>> E;
public:
	GraphList(){ n = e = 0; }
	~GraphList()
	{
		for (int i = 0; i < n; i++)
		{
			list<Edge<Te>*>::iterator it = E[i].begin();
			for (; it != E[i].end(); it++)
			{
				if (*it)
				{
					delete (*it);
					(*it) = NULL;
				}
			}
		}
	}

public:
	// Vertex
	virtual int Insert(Tv const& vData)
	{
		E.push_back(list<Edge<Te>*>());
		V.push_back(Vertex<Tv>(vData));
		n++;
		return n;
	}

	virtual Tv Remove(int vPos)
	{
		list<Edge<Te>*>::iterator it = E[vPos].begin();
		for (; it != E[vPos].end(); it++)
		{
			if (*it)
			{
				V[(*it)->_index]._inDegree--;
				delete (*it);
				(*it) = NULL;
				e--;
			}
		}
		E.erase(E.begin() + vPos);
		Tv vBak = V[vPos]._data;
		V.erase(V.begin() + vPos);
		n--;
		for (int i = 0; i < n; i++)
		{
			vector<list<Edge<Te>*>>::iterator node = E.begin() + i;
			it = (*node).begin();
			for (; it != (*node).end();)
			{
				if ((*it) && (*it)->_index == vPos)
				{
					delete (*it);
					it = (*node).erase(it);
					(*(V.begin()+i))._outDegree--;
					e--;
				}
				else
					it++;
			}
		}
		Update(vPos);
		return vBak;
	}

	virtual Tv& vertex(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._data;
	}

	virtual int InDegree(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._inDegree;
	}

	virtual int OutDegree(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._outDegree;
	}

	virtual int FirstNbr(int vPos)
	{
		return NextNbr(vPos, n);
	}

	virtual int NextNbr(int vPos, int uPos)
	{
		while ((uPos > -1) && (!Exists(vPos, --uPos))); return uPos;
	}

	virtual VStatus& Status(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._status;
	}

	virtual int& DTime(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._dTime;
	}

	virtual int& FTime(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._fTime;
	}

	virtual int& Parent(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._parent;
	}

	virtual int& Priority(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		return V[vPos]._priority;
	}


	// Edge
	virtual bool Exists(int vPos, int uPos)
	{
		if( vPos < 0 || vPos >= n || uPos < 0 || uPos >= n)
			return false;
		list<Edge<Te>*>::iterator it = E[vPos].begin();
		for (; it != E[vPos].end(); it++)
		{
			if ((*it) && (*it)->_index == uPos)
				return true;
		}
		return false;
	}

	virtual void Insert(Te const& eData, int eWeight, int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0 || Exists(vPos, uPos)) return;
		V[vPos]._outDegree++;
		V[uPos]._inDegree++;
		e++;
		E[vPos].push_back(new Edge<Te>(eData, eWeight, uPos));
	}

	virtual Te Remove(int vPos, int uPos)
	{
		if (!Exists(vPos, uPos)) assert(false);
		Te eBak;
		list<Edge<Te>*>::iterator it = E[vPos].begin();
		for (; it != E[vPos].end();it++)
		{
			if ((*it) && (*it)->_index == uPos)
			{
				eBak = (*it)->_data;
				V[vPos]._outDegree--;
				V[uPos]._inDegree--;
				e--;
				delete (*it);
				(*it) = NULL;
				E[vPos].erase(it);
				return eBak;
			}
		}
		assert(false);
	}

	virtual EStatus& Status(int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0)
			assert(false);
		list<Edge<Te>*>::iterator it = E[vPos].begin();
		for (; it != E[vPos].end(); it++)
		{
			if ((*it) && (*it)->_index == uPos)
				return (*it)->_status;
		}
		assert(false);
	}

	virtual Te& edge(int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0)
			assert(false);
		list<Edge<Te>*>::iterator it = E[vPos].begin();
		for (; it != E[vPos].end(); it++)
		{
			if ((*it) && (*it)->_index == uPos)
				return (*it)->_data;
		}
		assert(false);
	}

	virtual int& Weight(int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0)
			assert(false);
		list<Edge<Te>*>::iterator it = E[vPos].begin();
		for (; it != E[vPos].end(); it++)
		{
			if ((*it) && (*it)->_index == uPos)
				return (*it)->_weight;
		}
		assert(false);
	}

	int Index(int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0)
			assert(false);
		list<Edge<Te>*>::iterator it = E[vPos].begin();
		for (; it != E[vPos].end(); it++)
		{
			if ((*it) && (*it)->_index == uPos)
				return (*it)->_index;
		}
		assert(false);
	}

	void Update(int index)
	{
		for (unsigned int i = 0; i < E.size(); i++)
		{
			list<Edge<Te>*>::iterator it = E[i].begin();
			for (; it != E[i].end(); it++)
			{
				if ((*it) && (*it)->_index > index)
					(*it)->_index -= 1;
			}
		}
	}

	void PrintDegree()
	{
		int* indegree = new int[n];
		int* outdegree = new int[n];
		memset(indegree, 0, sizeof(int)*n);
		memset(outdegree, 0, sizeof(int)*n);
		for (unsigned int v = 0; v < E.size(); v++)
		{
			list<Edge<Te>*>::iterator it = (*(E.begin()+v)).begin();
			for (; it != (*(E.begin() + v)).end(); it++)
			{
				outdegree[v]++;
				int _index = (*it)->_index;
				if (_index >= n) assert(false);
				indegree[_index]++;
			}
		}
		cout << endl;
		for (int i = 0; i < n; i++)
		{
			cout << "Vertex:" << i << endl;
			cout << "inDegree:" << indegree[i] << "   " << "outDegree:" << outdegree[i] << endl;
			cout << endl;
		}

		delete[] indegree;
		delete[] outdegree;
	}
};

template<typename Tv, typename Te>
ostream& operator << (ostream& out, GraphList<Tv, Te>& c)
{
	out << "===========================Vertex===========================" << endl;
	out << "N:" << c.n << endl;
	out << "===========================Vertex===========================" << endl << endl;
	for (int i = 0; i < c.n; i++)
	{
		out << "Vertex:" << i << endl << c.V[i] << endl;
	}

	out << endl << endl << endl << endl << endl;
	out << "============================Edge=============================" << endl;
	out << "E:" << c.e << endl;
	out << "============================Edge=============================" << endl << endl;
	for (int i = 0; i < c.n; i++)
	{
		out << "Edge" << i << endl;
		list<Edge<Te>*>::iterator it = c.E[i].begin();
		for (; it != c.E[i].end(); it++)
		{
			out << *(*it) << endl;
		}
	}
	return out;
}

#endif