#ifndef _HUFFMAN_H__
#define _HUFFMAN_H__
#include "BinTree.h"
#include <list>
#include<vector>
#include<hash_map>

#define N_CHAR (0x80-0x20)

struct Huffchar
{
	char _c;
	int _weight;

	Huffchar();
	Huffchar(char c, int weight);
	Huffchar(int c, int weight);
	bool operator<(const Huffchar& hc)const;
	bool operator==(const Huffchar& hc)const;
	bool operator>(const Huffchar& hc)const;
	bool operator!=(const Huffchar& hc)const;
	bool operator<=(const Huffchar& hc)const;
	bool operator>=(const Huffchar& hc)const;
};

struct Huffcode
{
	vector<char> _code;
	void Set(char c = '1');
	void Clear();
	char Pop();
	void Concat(const Huffcode& code);
	int  Size()const;
	char operator [](int index)const;
};

typedef BinTree<Huffchar> Hufftree;
typedef BinNode<Huffchar> Huffnode;
typedef list<Hufftree*>	Huffforest;
typedef hash_map<char, Huffcode> Hufftable;

int* Statistics(const char* filename);
Huffforest* BuildForest(int* pFreq);
Hufftree* FindMinTree(Huffforest* pForest);
Hufftree* BuildTree(Huffforest* pForest);
void CreateCode(Hufftable* pTable, Huffcode* pCode, Huffnode* pNode);
Hufftable* BuildTable(Hufftree* pTree);
void PrintTable(Hufftable* pTable, const char* pTableFile);

Huffcode* Encode(Hufftable* pTable, const char* pFileName);
void Decode(Hufftree* pTree, Huffcode* pCode, const char* pFileOut);

bool CompareFile(const char* pFileName1, const char* pFileName2);

#endif