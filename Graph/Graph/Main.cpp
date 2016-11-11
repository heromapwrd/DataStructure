#include "GraphList.h"
#include "GraphMatrix.h"
#define VERTEXNUM 6
#define RANDEDGE  20
#define DELETEEDGE 6

struct Node
{
	int _num;
	char _letter;
	Node()
	{

	}

	Node(int n, char c) :_num(n), _letter(c)
	{

	}

	Node(const Node& node)
	{
		_num = node._num;
		_letter = node._letter;
	}
};

ostream& operator << (ostream& out, const Node& node)
{
	out << "Num: " << node._num << "  Letter: " << node._letter << " ";
	return out;
}

struct Connect
{
	int parent;
	int child;

	Connect()
	{

	}

	Connect(int p, int c) :parent(p), child(c)
	{

	}

	Connect(Connect const& c)
	{
		parent = c.parent;
		child = c.child;
	}
};

ostream& operator << (ostream& out, Connect const& c)
{
	out << "Parent: " << c.parent << "  Child: " << c.child << " ";
	return out;
}


void main()
{

	// ��������
	/*GraphMatrix<int, Connect> matrix;
	for (int i = 0; i < VERTEXNUM; i++)
	{
	matrix.Insert(i);
	}
	for (int i = 0; i < RANDEDGE; i++)
	{
	int v = rand() % VERTEXNUM;
	while (v < 0)
	v += VERTEXNUM;
	int u = rand() % VERTEXNUM;
	while (u < 0)
	u += VERTEXNUM;
	int w = rand() % (RANDEDGE/2);
	while (w < 0)
	w += RANDEDGE / 2;
	if (v != u)
	{
	Connect c = Connect(v, u);
	matrix.Insert(c, w, v, u);
	}

	}

	cout << matrix << endl;
	matrix.PrintDegree();

	for (int i = 0; i < DELETEEDGE; i++)
	{
	int v = rand() % VERTEXNUM;
	while (v < 0)
	v += VERTEXNUM;
	int u = rand() % VERTEXNUM;
	while (u < 0)
	u += VERTEXNUM;
	if (matrix.Exists(v,u))
	matrix.Remove(v, u);
	}
	matrix.Remove(matrix.n / 2);

	cout << matrix << endl;
	matrix.PrintDegree();*/

	//������ȱ���
	/*GraphList<Node, Connect> matrix;
	matrix.Insert(Node(0, 'S'));
	for (int i = 0; i < 7; i++)
	{
	matrix.Insert(Node(i + 1, 'A' + i));
	}
	matrix.Insert(Connect(0, 1), rand() % RANDEDGE, 0, 1);
	matrix.Insert(Connect(0, 3), rand() % RANDEDGE, 0, 3);
	matrix.Insert(Connect(0, 4), rand() % RANDEDGE, 0, 4);
	matrix.Insert(Connect(1, 3), rand() % RANDEDGE, 1, 3);
	matrix.Insert(Connect(1, 5), rand() % RANDEDGE, 1, 5);
	matrix.Insert(Connect(3, 2), rand() % RANDEDGE, 3, 2);
	matrix.Insert(Connect(4, 2), rand() % RANDEDGE, 4, 2);
	matrix.Insert(Connect(5, 6), rand() % RANDEDGE, 5, 6);
	matrix.Insert(Connect(5, 7), rand() % RANDEDGE, 5, 7);
	matrix.Insert(Connect(7, 2), rand() % RANDEDGE, 7, 2);
	matrix.Insert(Connect(7, 6), rand() % RANDEDGE, 7, 6);
	matrix.BFS(0);
	cout << matrix << endl;*/

	//������ȱ���
	/*GraphList<Node, Connect> matrix;
	for (int i = 0; i < 7; i++)
	{
	matrix.Insert(Node(i , 'A' + i));
	}
	matrix.Insert(Connect(0, 5), rand() % RANDEDGE, 0, 5);
	matrix.Insert(Connect(0, 2), rand() % RANDEDGE, 0, 2);
	matrix.Insert(Connect(0, 1), rand() % RANDEDGE, 0, 1);
	matrix.Insert(Connect(1, 2), rand() % RANDEDGE, 1, 2);
	matrix.Insert(Connect(3, 4), rand() % RANDEDGE, 3, 4);
	matrix.Insert(Connect(3, 0), rand() % RANDEDGE, 3, 0);
	matrix.Insert(Connect(4, 5), rand() % RANDEDGE, 4, 5);
	matrix.Insert(Connect(5, 6), rand() % RANDEDGE, 5, 6);
	matrix.Insert(Connect(6, 2), rand() % RANDEDGE, 6, 2);
	matrix.Insert(Connect(6, 0), rand() % RANDEDGE, 6, 0);
	matrix.DFS_I(0);
	cout << matrix << endl;*/

	// ��������

	/*GraphMatrix<Node, Connect> matrix;
	for (int i = 0; i < 6; i++)
	{
		matrix.Insert(Node(i, 'A' + i));
	}
	matrix.Insert(Connect(0, 2), rand() % RANDEDGE, 0, 2);
	matrix.Insert(Connect(0, 3), rand() % RANDEDGE, 0, 3);
	matrix.Insert(Connect(1, 2), rand() % RANDEDGE, 1, 2);
	matrix.Insert(Connect(2, 3), rand() % RANDEDGE, 2, 3);
	matrix.Insert(Connect(2, 4), rand() % RANDEDGE, 2, 4);
	matrix.Insert(Connect(2, 5), rand() % RANDEDGE, 2, 5);
	matrix.Insert(Connect(4, 5), rand() % RANDEDGE, 4, 5);
	stack<Node>* s = matrix.TSort(2);
	cout << matrix << endl;
	cout << endl << endl;
	while (!s->empty())
	{
		cout << s->top() << endl;
		cout << endl;
		s->pop();
	}
	delete s;
	*/
	/*GraphList<Node, Connect> matrix;
	for (int i = 0; i < 6; i++)
	{
	matrix.Insert(Node(i, 'A' + i));
	}
	matrix.Insert(Connect(0, 2), rand() % RANDEDGE, 0, 2);
	matrix.Insert(Connect(0, 5), rand() % RANDEDGE, 0, 5);
	matrix.Insert(Connect(1, 2), rand() % RANDEDGE, 1, 2);
	matrix.Insert(Connect(2, 5), rand() % RANDEDGE, 2, 5);
	matrix.Insert(Connect(2, 4), rand() % RANDEDGE, 2, 4);
	matrix.Insert(Connect(2, 3), rand() % RANDEDGE, 2, 3);
	matrix.Insert(Connect(4, 3), rand() % RANDEDGE, 4, 3);
	list<int>* s = matrix.TSort_Extremum();
	cout << matrix << endl;
	cout << endl << endl;
	if (s)
	{
	while (!s->empty())
	{
	int v = s->front();
	cout << matrix.vertex(v) << endl << endl;
	s->pop_front();
	}
	delete s;
	}*/
	
	//˫��ͨ��
	GraphList<Node, Connect> matrix;
	for (int i = 0; i < 6; i++)
	{
	matrix.Insert(Node(i, 'A' + i));
	}
	matrix.Insert(Connect(0, 2), rand() % RANDEDGE, 0, 2);
	matrix.Insert(Connect(0, 5), rand() % RANDEDGE, 0, 5);
	matrix.Insert(Connect(1, 2), rand() % RANDEDGE, 1, 2);
	matrix.Insert(Connect(2, 5), rand() % RANDEDGE, 2, 5);
	matrix.Insert(Connect(2, 4), rand() % RANDEDGE, 2, 4);
	matrix.Insert(Connect(2, 3), rand() % RANDEDGE, 2, 3);
	matrix.Insert(Connect(4, 3), rand() % RANDEDGE, 4, 3);
	matrix.BCC(0);
	cout << matrix << endl;
	cout << endl << endl;
}