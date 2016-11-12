#ifndef _GRAPH_H__
#define _GRAPH_H__
#include "Common.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <hash_map>
#include <assert.h>
using namespace std;

template<typename Tv, typename Te>
class Graph;
template<typename Tv, typename Te>
struct PrimPU;

template<typename Tv, typename Te>
struct DijkstraPU;

template<typename Tv,typename Te>
class Graph
{
public:
	int n;
	virtual int Insert(Tv const& vData) = 0;
	virtual Tv Remove(int vPos) = 0;
	virtual Tv& vertex(int vPos) = 0;
	virtual int InDegree(int vPos) = 0;
	virtual int OutDegree(int vPos) = 0;
	virtual int FirstNbr(int vPos) = 0;
	virtual int NextNbr(int vPos, int uPos) = 0;
	virtual VStatus& Status(int vPos) = 0;
	virtual int& DTime(int vPos) = 0;
	virtual int& FTime(int vPos) = 0;
	virtual int& Parent(int vPos) = 0;
	virtual int& Priority(int vPos) = 0;

	int e;
	virtual bool Exists(int vPos, int uPos) = 0;
	virtual void Insert(Te const& eData, int eWeight, int vPos, int uPos) = 0;
	virtual Te Remove(int vPos, int uPos) = 0;
	virtual EStatus& Status(int vPos, int uPos) = 0;
	virtual Te& edge(int vPos, int uPos) = 0;
	virtual int& Weight(int vPos, int uPos) = 0;

	void BFS(int pos);
	void DFS(int pos);
	void DFS_I(int pos);
	hash_map<int,int>* BCC(int pos);
	stack<Tv>* TSort(int pos);
	list<int>* TSort_Extremum();
	void Prim(int pos);
	void DIJKSTRA(int pos);
	template <typename PU> void pfs(int pos, PU prioUpdater);

private:
	void Reset()
	{
		for (int i = 0; i < n; i++)
		{
			Status(i) = UNDISCOVERED; DTime(i) = FTime(i) = -1;
			Parent(i) = -1; Priority(i) = INT_MAX;
			for (int j = 0; j < n; j++)
				if (Exists(i, j)) Status(i, j) = UNDETERMINED;
		}
	}

	void BFS(int vPos, int& clock);
	void DFS(int vPos, int& clock);
	void DFS_I(int vPos, int& clock);
	void BCC(int v, int& clock, stack<int>& s, hash_map<int,int>* result);
	bool TSort(int v, int& clock, stack<Tv>* S);
	template <typename PU> void PFS(int v, PU prioUpdater);
};

template<typename Tv, typename Te>
struct PrimPU 
{
	virtual void operator()(Graph<Tv, Te>* g, int v, int u)
	{
		if (g->Status(u) == UNDISCOVERED)
		{
			if (g->Priority(u) > g->Weight(v, u))
			{
				g->Priority(u) = g->Weight(v, u);
				g->Parent(u) = v;
			}
		}
	}
};


template<typename Tv, typename Te>
struct DijkstraPU
{
	virtual void operator()(Graph<Tv, Te>* g, int v, int u)
	{
		if (g->Status(u) == UNDISCOVERED)
		{
			if (g->Priority(u) > (g->Weight(v, u) + g->Priority(v)))
			{
				g->Priority(u) = g->Weight(v, u) + g->Priority(v);
				g->Parent(u) = v;
			}
		}
	}
};

template<typename Tv,typename Te>
template <typename PU>
void Graph<Tv,Te>::PFS(int v, PU prioUpdater)
{
	Priority(v) = 0; Status(v) = VISITED; Parent(v) = -1;
	while (true)
	{
		for (int u = FirstNbr(v); u > -1; u = NextNbr(v,u))
		{
			prioUpdater(this, v, u);
		}
		for (int i = 0,shortest = INT_MAX; i < n; i++)
		{
			if (UNDISCOVERED == Status(i))
			{
				if (shortest > Priority(i))
				{
					shortest = Priority(i);
					v = i;
				}
			}
			
		}
		if (VISITED == Status(v)) break;
		Status(v) = VISITED; Status(Parent(v), v) = TREE;
	}
}

template<typename Tv, typename Te>
bool Graph<Tv, Te>::TSort(int v, int& clock, stack<Tv>* S)
{
	Status(v) = DISCOVERED; DTime(v) = ++clock;
	for (int u = FirstNbr(v); u > -1; u = NextNbr(v, u))
	{
		switch(Status(u))
		{
		case UNDISCOVERED:
			Status(v, u) = TREE; Parent(u) = v;
			if (!TSort(u, clock, S))
				return false;
			break;
		case DISCOVERED:
			Status(v, u) = BACKWARD;
			return false;
			break;
		case VISITED:
			if (DTime(v) < DTime(u))
				Status(v, u) = FORWARD;
			else
				Status(v, u) = CROSS;
			break;
		}
	}
	Status(v) = VISITED; FTime(v) = clock++; S->push(vertex(v));
	return true;
}

#define Hca(x) FTime(x)
template<typename Tv, typename Te>
void Graph<Tv,Te>::BCC(int v, int& clock, stack<int>& s, hash_map<int,int>* result)
{
	Status(v) = DISCOVERED; Hca(v) = DTime(v) = ++clock; s.push(v);
	for (int u = FirstNbr(v); u > -1; u = NextNbr(v, u))
	{
		switch (Status(u))
		{
		case UNDISCOVERED:
			Parent(u) = v; Status(v, u) = TREE; BCC(u, clock, s, result);
			if (Hca(u) < DTime(v))
				Hca(v) = min(Hca(v), Hca(u));
			else
			{
				result->insert(pair<int, int>(v, v));
				while (v != s.top())
					s.pop();
				s.push(v);
			}
			break;
		case DISCOVERED:
			Status(v, u) = BACKWARD;
			if (u != Parent(v)) Hca(v) = min(Hca(v), DTime(u));
			break;
		case VISITED:
			Status(v, u) = (DTime(v) < DTime(u)) ? FORWARD : CROSS;
			break;
		}
	}
	Status(v) = VISITED; 
}
#undef Hca

template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS_I(int vPos, int& clock)
{
	if (vPos >= n || vPos < 0)
		return;
	stack<int> s;
	int* memory = new int[n];
	for (int i = 0; i < n; i++)
		memory[i] = n;
	Status(vPos) = DISCOVERED; DTime(vPos) = ++clock;
	s.push(vPos);
	while (!s.empty())
	{
		int v = s.top();
		int u = NextNbr(v, memory[v]);
		memory[v] = u;
		if (u < 0)
		{
			FTime(v) = ++clock;
			Status(v) = VISITED;
			s.pop();
			continue;
		}

		switch (Status(u))
		{
		case UNDISCOVERED:
			Status(u) = DISCOVERED; DTime(u) = ++clock; Parent(u) = v; 
			Status(v, u) = TREE;
			s.push(u);
			break;
		case DISCOVERED:
			Status(v, u) = BACKWARD;
			break;
		case VISITED:
			if (DTime(v) < DTime(u))
				Status(v, u) = FORWARD;
			else
				Status(v, u) = CROSS;
			break;
		}
	}
	delete[] memory;
}


template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int vPos, int& clock)
{
	Status(vPos) = DISCOVERED; DTime(vPos) = ++clock;
	for (int u = FirstNbr(vPos); u > -1; u = NextNbr(vPos, u))
	{
		switch (Status(u))
		{
		case UNDISCOVERED:
			Parent(u) = vPos; Status(vPos, u) = TREE; DFS(u, clock);
			break;
		case DISCOVERED:
			Status(vPos, u) = BACKWARD;
			break;
		default:
			if (DTime(vPos) < DTime(u))
				Status(vPos, u) = FORWARD;
			else
				Status(vPos, u) = CROSS;
			break;
		}
	}
	Status(vPos) = VISITED; FTime(vPos) = ++clock;
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int vPos, int& clock)
{
	queue<int> q;
	Status(vPos) = DISCOVERED; q.push(vPos);
	while (!q.empty())
	{
		int v = q.front(); q.pop();
		DTime(v) = ++clock;
		for (int u = FirstNbr(v); u > -1; u = NextNbr(v, u))
		{
			switch (Status(u))
			{
			case UNDISCOVERED:
				q.push(u); 
				Status(u) = DISCOVERED; 
				Parent(u) = v;
				Status(v, u) = TREE;
				break;
			default:
				Status(v, u) = CROSS;
				break;
			}
		}
		Status(v) = VISITED;
		FTime(v) = ++clock;
	}
}

template<typename Tv,typename Te>
template <typename PU> 
void Graph<Tv, Te>::pfs(int pos, PU prioUpdater)
{
	Reset();
	int v = pos; int clock = 0;
	do 
	{
		if (UNDISCOVERED == Status(v))
			PFS(v, prioUpdater);
	} while (pos!=(v = (++v%n)));

}

template<typename Tv, typename Te>
void Graph<Tv, Te>::DIJKSTRA(int pos)
{

}

template<typename Tv, typename Te>
void Graph<Tv, Te>::Prim(int pos)
{

}

template<typename Tv,typename Te>
list<int>* Graph<Tv,Te>::TSort_Extremum()
{
	Reset();
	int v;
	bool bExist = false;
	for (int i = 0; i < n; i++)
	{
		if (!InDegree(i))
		{
			bExist = true;
			v = i;
			break;
		}
	}

	if (!bExist)
		return NULL;
	list<int>* l = new list<int>;
	int* indegree = new int[n];
	memset(indegree, 0, sizeof(int)*n);

	do 
	{
		if (find(l->begin(), l->end(), v) == l->end() && (0 == InDegree(v) || InDegree(v) == indegree[v]))
		{
			l->push_back(v);
			for (int u = FirstNbr(v); u > -1; u = NextNbr(v, u))
			{
				indegree[u]++;
			}
		}

		v = ((++v)%n);
		
	} while (n!=l->size());
	delete[] indegree;
	return l;
}

template<typename Tv, typename Te>
stack<Tv>* Graph<Tv, Te>::TSort(int pos)
{
	Reset();
	int clock = 0;
	int v = pos;
	stack<Tv> *s = new stack<Tv>;
	do 
	{
		if (UNDISCOVERED == Status(v))
		{
			if (!TSort(v, clock, s))
			{
				while (!s->empty())
					s->pop();
				break;
			}
		}
		
	} while (pos!=(v = (++v%n)));
	return s;
}

template<typename Tv, typename Te>
hash_map<int,int>* Graph<Tv, Te>::BCC(int pos)
{
	Reset();
	int v = pos; int clock = 0; 
	stack<int> s;
	hash_map<int,int>* l = new hash_map<int,int>;
	do 
	{
		if (UNDISCOVERED == Status(v))
		{
			Parent(v) = -1;
			BCC(v, clock, s, l);
			s.pop();
		}
	} while (pos!=(v=(++v%n)));
	return l;
}

template<typename Tv,typename Te>
void Graph<Tv,Te>::DFS_I(int pos)
{
	Reset();
	int s = pos; int clock = 0;
	do
	{
		if (UNDISCOVERED == Status(s))
			DFS_I(s, clock);
	} while (pos != (s = (++s%n)));
}


template<typename Tv, typename Te>
void Graph<Tv, Te>::DFS(int pos)
{
	Reset();
	int s = pos; int clock = 0;
	do
	{
		if (UNDISCOVERED == Status(s))
			DFS(s, clock);
	} while (pos != (s = (++s%n)));
}

template<typename Tv, typename Te>
void Graph<Tv, Te>::BFS(int pos)
{
	Reset();
	int s = pos; int clock = 0;
	do 
	{
		if (UNDISCOVERED == Status(s))
		BFS(s, clock);
	} while (pos!=(s = (++s%n)));
}

#endif