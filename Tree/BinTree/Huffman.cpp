#include "Huffman.h"
#include <iostream>
#include <assert.h>

Huffchar::Huffchar() :_c('^'), _weight(0)
{

}

Huffchar::Huffchar(char c, int weight) :_c(c), _weight(weight)
{

}

Huffchar::Huffchar(int c, int weight) : _c(c), _weight(weight)
{

}


bool Huffchar::operator <(const Huffchar& hc)const
{
	return _weight < hc._weight;
}

bool Huffchar::operator==(const Huffchar& hc)const
{
	return _weight == hc._weight;
}

bool Huffchar::operator>(const Huffchar& hc)const
{
	return _weight > hc._weight;
}

bool Huffchar::operator!=(const Huffchar& hc)const
{
	return _weight != hc._weight;
}

bool Huffchar::operator<=(const Huffchar& hc)const
{
	return _weight <= hc._weight;
}

bool Huffchar::operator>=(const Huffchar& hc)const
{
	return _weight >= hc._weight;
}

void Huffcode::Set(char c)
{
	_code.push_back(c);
}

void Huffcode::Clear()
{
	_code.push_back('0');
}

char Huffcode::Pop()
{
	char c;
	c = _code.back();
	_code.pop_back();
	return c;
}

void Huffcode::Concat(const Huffcode& code)
{
	for (int i = 0; i < code._code.size();i++)
	{
		_code.push_back(code._code[i]);
	}
}

int Huffcode::Size()const
{
	return _code.size();
}

char Huffcode::operator [](int index)const
{
	if (index >= _code.size())
		return 0;
	return _code[index];
}

int* Statistics(const char* filename)
{
	FILE* pFile = NULL;
	pFile = fopen(filename, "rt");
	if (!pFile)
		return NULL;
	int * pFreq = new int[N_CHAR];
	memset(pFreq, 0, N_CHAR*sizeof(int));
	char c;
	while (fscanf(pFile, "%c", &c) > 0)
	{
		if (c >= 0x20 && c < 0x80)
		{
			pFreq[c - 0x20]++;
		}
	}
	fclose(pFile);
	return pFreq;
}

Huffforest* BuildForest(int* pFreq)
{
	if (!pFreq)
		return NULL;
	Huffforest* pForest = new Huffforest;
	for (int i = 0; i < N_CHAR;i++)
	{
		Hufftree* pTree = new Hufftree();
		pTree->InsertAsRoot(Huffchar(i + 0x20, pFreq[i]));
		pForest->push_back(pTree);
	}
	return pForest;
}

Hufftree* FindMinTree(Huffforest* pForest)
{
	if (!pForest || pForest->empty())
		return NULL;
	Huffforest::iterator it = pForest->begin();
	Huffforest::iterator iterMin = it;
	for (it++; it != pForest->end(); it++)
	{
		if ((*it)->Root()->_data < (*iterMin)->Root()->_data)
			iterMin = it;
	}
	Hufftree* pMinTree = *iterMin;
	pForest->erase(iterMin);
	return pMinTree;
}

Hufftree* BuildTree(Huffforest* pForest)
{
	if (!pForest)
		return NULL;
	while (pForest->size()>1)
	{
		Hufftree* pMinTree1 = FindMinTree(pForest);
		Hufftree* pMinTree2 = FindMinTree(pForest);
		if (!pMinTree1 || !pMinTree2)
			assert(false);
		Hufftree* pTree = new Hufftree;
		pTree->InsertAsRoot(Huffchar('^', pMinTree1->Root()->_data._weight + pMinTree2->Root()->_data._weight));
		pTree->AttachAsLC(pTree->Root(), pMinTree1);
		pTree->AttachAsRC(pTree->Root(), pMinTree2);
		pForest->push_back(pTree);
	}
	if (pForest->size() <= 0)
		return NULL;
	
	Hufftree* pTree = pForest->front();
	return pTree;
}

void CreateCode(Hufftable* pTable, Huffcode* pCode, Huffnode* pNode)
{
	if (!pTable || !pCode || !pNode)
		return;
	if (pNode->IsLeaf())
	{
		pTable->insert(pair<char, Huffcode>(pNode->_data._c, *pCode));
	}
	if (pNode->HasLChild())
	{
		pCode->Clear();
		CreateCode(pTable, pCode, pNode->_lChild);
	}
	if (pNode->HasRChild())
	{
		pCode->Set();
		CreateCode(pTable, pCode, pNode->_rChild);
	}
	if (!pNode->IsRoot())
		pCode->Pop();
}

Hufftable* BuildTable(Hufftree* pTree)
{
	if (!pTree)
		return NULL;
	Hufftable* pTable = new Hufftable;
	Huffcode* pCode = new Huffcode;
	CreateCode(pTable, pCode, pTree->Root());
	if (pCode)
	{
		delete pCode;
		pCode = NULL;
	}
	return pTable;
}

void PrintTable(Hufftable* pTable, const char* pTableFile)
{
	if (!pTable || !pTableFile)
		return;
	FILE* pFile = NULL;
	pFile = fopen(pTableFile, "wt");
	if (!pFile)
		return;
	Hufftable::iterator it = pTable->begin();
	int iCount = 0;
	for (; it != pTable->end(); it++)
	{
		char c = it->first;
		fwrite(&c, sizeof(c), 1, pFile);
		c = ':';
		fwrite(&c, sizeof(c), 1, pFile);
		c = ' ';
		fwrite(&c, sizeof(c), 1, pFile);
		Huffcode code = it->second;
		for (int i = 0; i < code.Size(); i++)
		{
			c = code[i];
			fwrite(&c, sizeof(c), 1, pFile);
		}
		c = ' ';
		fwrite(&c, sizeof(c), 1, pFile);
		iCount++;
		int count = iCount;
		stack<char> s;
		do 
		{
			c = '0' + count % 10;
			s.push(c);
			count /= 10;
		} while (count>0);

		while (s.size() > 0)
		{
			c = s.top();
			s.pop();
			fwrite(&c, sizeof(c), 1, pFile);
		}
			
		c = '\n';
		fwrite(&c, sizeof(c), 1, pFile);
	}

	fclose(pFile);
}


Huffcode* Encode(Hufftable* pTable, const char* pFileName)
{
	if (!pTable)
		return NULL;
	FILE* pFile = NULL;
	pFile = fopen(pFileName, "rt");
	if (!pFile)
		return NULL;
	Huffcode* pCode = new Huffcode;
	char c;
	fread(&c, sizeof(c), 1, pFile);
	while (!feof(pFile))
	{
		Hufftable::iterator it = pTable->find(c);
		fread(&c, sizeof(c), 1, pFile);
		if (it == pTable->end())
			continue;
		pCode->Concat(it->second);
	}
	fclose(pFile);
	return pCode;
}

void Decode(Hufftree* pTree, Huffcode* pCode, const char* pFileOut)
{
	if (!pTree || !pCode)
		return;
	FILE * pFile = NULL;
	pFile = fopen(pFileOut, "wt");
	if (!pFile)
		return;
	Huffnode* pNode = pTree->Root();
	for (int i = 0; i < pCode->Size(); i++)
	{
		switch ((*pCode)[i])
		{
		case '0':
			pNode = pNode->_lChild;
			break;
		case '1':
			pNode = pNode->_rChild;
			break;
		}
		if (pNode->IsLeaf())
		{
			char c = pNode->_data._c;
			fwrite(&c, sizeof(c), 1, pFile);
			pNode = pTree->Root();
		}
	}
	fclose(pFile);
}

bool CompareFile(const char* pFileName1, const char* pFileName2)
{
	FILE* pFile1 = NULL;
	FILE* pFile2 = NULL;
	pFile1 = fopen(pFileName1, "rt");
	if (!pFile1)
		return false;
	pFile2 = fopen(pFileName2, "rt");
	if (!pFile2)
	{
		fclose(pFile1);
		return false;
	}
	char c1 = fgetc(pFile1);
	char c2 = fgetc(pFile2);
	while ((!feof(pFile1)) && (!feof(pFile2)))
	{
		if (c1!=c2)
		{
			fclose(pFile1);
			fclose(pFile2);
			return false;
		}
		c1 = fgetc(pFile1);
		c2 = fgetc(pFile2);
	}
	if (feof(pFile1) && feof(pFile2))
	{
		fclose(pFile1);
		fclose(pFile2);
		return true;
	}
	fclose(pFile1);
	fclose(pFile2);
	return false;
}
