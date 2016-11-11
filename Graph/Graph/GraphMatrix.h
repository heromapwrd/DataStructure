#ifndef _GRAPHMATRIX_H__
#define _GRAPHMATRIX_H__
#include "Graph.h"
#include <vector>
#include<assert.h>
using namespace std;


template<typename Tv,typename Te>
class GraphMatrix:public Graph<Tv,Te>
{
public:
	vector<Vertex<Tv>> V;
	vector<vector<Edge<Te>*>> E;
public:
	GraphMatrix(){ n = e = 0; }
	~GraphMatrix()
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (E[i][j])
				{
					delete E[i][j];
					E[i][j] = NULL;
				}
			}
		}
	}
	
public:
	// Vertex
	virtual int Insert(Tv const& vData)
	{
		for (int i = 0; i < n; i++)
		{
			E[i].push_back(NULL);
		}
		n++;
		E.push_back(vector<Edge<Te>*>(n,NULL));
		V.push_back(Vertex<Tv>(vData)); 
		return n;
	}

	virtual Tv Remove(int vPos)
	{
		if (vPos >= n || vPos < 0)
			assert(false);
		
		for (int j = 0; j < n; j++)
		{
			if (Exists(vPos, j))
			{
				delete E[vPos][j];
				E[vPos][j] = NULL;
				V[j]._inDegree--;
				e--;
			}
		}
		vector<vector<Edge<Te>*>>::iterator it = E.begin() + vPos;
		E.erase(it);
		n--; 
		Tv vBak = V[vPos]._data;
		V.erase(V.begin() + vPos);
		for (int i = 0; i < n; i++)
		{
			if (Exists(i, vPos))
			{
				delete E[i][vPos];
				E[i][vPos] = NULL;
				V[i]._outDegree--;
				e--;
			}
			E[i].erase(E[i].begin() + vPos);
		}
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
		return (vPos >= 0) && (vPos < n) && (uPos >= 0) && (uPos < n) && (E[vPos][uPos] != NULL);
	}

	virtual void Insert(Te const& eData, int eWeight, int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0 || Exists(vPos, uPos)) return;
		E[vPos][uPos] = new Edge<Te>(eData, eWeight, uPos);
		e++;
		V[vPos]._outDegree++;
		V[uPos]._inDegree++;
	}

	virtual Te Remove(int vPos, int uPos)
	{
		if (!Exists(vPos, uPos)) assert(false);
		Te eBak = E[vPos][uPos]->_data;
		delete E[vPos][uPos];
		E[vPos][uPos] = NULL;
		e--;
		V[vPos]._outDegree--;
		V[uPos]._inDegree--;
		return eBak;
	}

	virtual EStatus& Status(int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0 || !Exists(vPos,uPos))
			assert(false);
		return E[vPos][uPos]->_status;
	}

	virtual Te& edge(int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0 || !Exists(vPos, uPos))
			assert(false);
		return E[vPos][uPos]->_data;
	}

	virtual int& Weight(int vPos, int uPos)
	{
		if (vPos >= n || vPos < 0 || uPos >= n || uPos < 0 || !Exists(vPos, uPos))
			assert(false);
		return E[vPos][uPos]->_weight;
	}

public:
	void PrintDegree()
	{
		int* in = new int[n];
		int* out = new int[n];
		memset(in, 0, sizeof(int)*n);
		memset(out, 0, sizeof(int)*n);
		for (unsigned int v = 0; v < E.size(); v++)
		{
			for (unsigned int u = 0; u < E[v].size(); u++)
			{
				if (E[v][u])
				{
					in[u]++;
					out[v]++;
				}
			}
		}
		cout << endl;
		for (int i = 0; i < n; i++)
		{
			cout << "Vertex:" << i << endl;
			cout << "inDegree:" << in[i] << "   " << "outDegree:" << out[i] << endl;
			cout << endl;
		}
		delete[] in;
		delete[] out;
	}
};

template<typename Tv, typename Te>
ostream& operator << (ostream& out,  GraphMatrix<Tv, Te>&  c)
{
	out << "===========================Vertex===========================" << endl;
	out << "Vertex:" << c.n << endl;
	out << "===========================Vertex===========================" << endl << endl;
	for (int i = 0; i < c.n; i++)
	{
		out << "Vertex:" << i << endl << c.V[i] << endl;
	}

	out << endl << endl << endl << endl;
	out << "============================Edge=============================" << endl;
	out << "Edge:" << c.e << endl;
	out << "============================Edge=============================" << endl << endl;
	for (int v = 0; v < c.n; v++)
	{
		out << "Edge" << v << endl;
		for (int u = 0; u < c.n; u++)
		{
			if (c.Exists(v, u))
			{
				out << *(c.E[v][u]) << endl;
			}
		}
	}
	return out;
}

#endif