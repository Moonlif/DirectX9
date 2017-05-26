#pragma once
class cNode;
class cOpenList
{
public:
	cOpenList();
	~cOpenList();

private:
	std::vector<cNode*> m_vecHeap;

public:
	void Insert(cNode* pNode);
	cNode* MinFNode();
	void UpdateUpper(cNode* pNode);
	void UpdateLower(cNode* pNode);
	void Swap(cNode* pNode1, cNode* pNode2);
};

