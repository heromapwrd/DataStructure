#include "Huffman.h"

int main(int argc,char* argv[])
{
	if (argc < 4)
		return 0;
	const char* samplefile = argv[1];
	const char* infile = argv[2];
	const char* outfile = argv[3];
	int* pFreq = Statistics(samplefile);
	Huffforest* pForest = BuildForest(pFreq);
	Hufftree* pTree = BuildTree(pForest);
	Hufftable* pTable = BuildTable(pTree);
	PrintTable(pTable, "table.txt");
	Huffcode* pCode = Encode(pTable, infile);
	Decode(pTree, pCode, outfile);
	bool bIsEqual = CompareFile(infile, outfile);
	if (pFreq)
		delete[] pFreq;
	if (pForest)
		delete pForest;
	if (pTree)
		delete pTree;
	if (pTable)
		delete pTable;
	if (pCode)
		delete pCode;
}