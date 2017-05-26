#include "stdafx.h"
#include "cOpenList.h"

#include "cNode.h"

cOpenList::cOpenList()
{
}


cOpenList::~cOpenList()
{
}

void cOpenList::Insert(cNode * pNode)
{
	pNode->m_nHeapIndex = m_vecHeap.size();
	m_vecHeap.push_back(pNode);
	UpdateUpper(pNode);
}

cNode * cOpenList::MinFNode()
{
	if (m_vecHeap.empty()) return NULL;
	cNode* pMinFNode = m_vecHeap[0];
	m_vecHeap[0] = m_vecHeap.back();
	m_vecHeap[0]->m_nHeapIndex = 0;

	m_vecHeap.pop_back();

	if (!m_vecHeap.empty())
	{
		UpdateLower(m_vecHeap[0]);
	}

	return pMinFNode;
}

void cOpenList::UpdateUpper(cNode * pNode)
{
	if (pNode->m_nHeapIndex == 0) return;
	int nParent = (pNode->m_nHeapIndex - 1) / 2;

	if (m_vecHeap[nParent]->m_fF > pNode->m_fF)
	{
		Swap(pNode, m_vecHeap[nParent]);
		UpdateUpper(pNode);
	}
}

void cOpenList::UpdateLower(cNode * pNode)
{
	int nLChild = pNode->m_nHeapIndex * 2 + 1;
	if (nLChild >= m_vecHeap.size())
		return;

	cNode* pMinCHild = m_vecHeap[nLChild];
	int nRchild = pNode->m_nHeapIndex * 2 + 2;
	if (nRchild < m_vecHeap.size())
	{
		if (m_vecHeap[nRchild]->m_fF < pMinCHild->m_fF)
		{
			pMinCHild = m_vecHeap[nRchild];
		}
	}

	if (pNode->m_fF > pMinCHild->m_fF)
	{
		Swap(pNode, pMinCHild);
		UpdateLower(pNode);
	}
}

void cOpenList::Swap(cNode * pNode1, cNode * pNode2)
{
	m_vecHeap[pNode1->m_nHeapIndex] = pNode2;
	m_vecHeap[pNode2->m_nHeapIndex] = pNode1;

	int nTemp = pNode1->m_nHeapIndex;
	pNode1->m_nHeapIndex = pNode2->m_nHeapIndex;
	pNode2->m_nHeapIndex = nTemp;
}
