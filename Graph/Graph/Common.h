#ifndef _COMMON_H__
#define _COMMON_H__
#include <iostream>
using namespace std;

enum VStatus
{
	UNDISCOVERED,
	DISCOVERED,
	VISITED
};

#define STRING(x) #x
const char* VSTATUS[] = 
{
	STRING(UNDISCOVERED),
	STRING(DISCOVERED),
	STRING(VISITED)
};

enum EStatus
{
	UNDETERMINED,
	TREE,
	CROSS,
	FORWARD,
	BACKWARD
};

const char* ESTATUS[] = 
{
	STRING(UNDETERMINED),
	STRING(TREE),
	STRING(CROSS),
	STRING(FORWARD),
	STRING(BACKWARD)
};

template<typename Tv>
struct Vertex
{
	Tv _data; int _inDegree; int _outDegree;
	int _dTime; int _fTime;
	VStatus _status;
	int _parent; int _priority;
	Vertex(Tv const& data = (Tv)0) :_data(data), _inDegree(0), _outDegree(0),
		_status(UNDISCOVERED), _dTime(-1), _fTime(-1), _parent(-1), _priority(INT_MAX)
	{

	}
};



template<typename Tv>
ostream& operator <<(ostream& out, Vertex<Tv> const& c)
{
	out << "--------------------------------------------------------" << endl;
	out << "data:" << c._data << endl;
	out << "inDegree:" << c._inDegree << "  " << "outDegree:" << c._outDegree << "  "
		<< "dTime:" << c._dTime << "  " << "fTime:" << c._fTime << "  " << "Status:" << VSTATUS[c._status] << "  "
		<< "Parent:" << c._parent << "  " << "Priority:" << c._priority << endl;
	return out;
}

template<typename Te>
struct Edge
{
	Te _data; int _weight; EStatus _status; int _index;
	Edge(Te const& data, int w, int index) :_data(data), _weight(w), _index(index), _status(UNDETERMINED)
	{

	}
};

template<typename Te>
ostream& operator <<(ostream& out, Edge<Te> const& c)
{
	out << "---------------------------------------------------------------" << endl;
	out << "_data(" << c._data << ")" << endl;
	out << "Weight:" << c._weight << "  " << "Status:" << ESTATUS[c._status] <<"  " << "Index:" << c._index << endl;
	return out;
}

#endif