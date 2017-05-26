#include "stdafx.h"
#include "cAstarMainGame.h"

#include "cOpenList.h"
const float Distance_cost = 1.0f;
const float Diagonal_cost = 1.4f;

cAstarMainGame::cAstarMainGame()
{
}


cAstarMainGame::~cAstarMainGame()
{
	for each (auto p in m_vecNode)
		delete p;

	delete m_pOpenList;
}

void cAstarMainGame::Setup()
{
	m_pOpenList = new cOpenList;
	std::map<char, cNode::eNodeType> mapNodeType;

	mapNodeType['E'] = cNode::E_EMPTY;
	mapNodeType['B'] = cNode::E_WALL;
	mapNodeType['S'] = cNode::E_START;
	mapNodeType['D'] = cNode::E_DEST;

	m_vecDirection.push_back(E_LEFT);
	m_vecDirection.push_back(E_RIGHT);
	m_vecDirection.push_back(E_UP);
	m_vecDirection.push_back(E_DOWN);
	m_vecDirection.push_back(E_LEFT | E_UP);
	m_vecDirection.push_back(E_RIGHT | E_UP);
	m_vecDirection.push_back(E_LEFT | E_DOWN);
	m_vecDirection.push_back(E_RIGHT | E_DOWN);

	m_mapDistance[E_LEFT] = Distance_cost;
	m_mapDistance[E_RIGHT] = Distance_cost;
	m_mapDistance[E_UP] = Distance_cost;
	m_mapDistance[E_DOWN] = Distance_cost;
	m_mapDistance[E_LEFT | E_UP] = Diagonal_cost;
	m_mapDistance[E_RIGHT | E_UP] = Diagonal_cost;
	m_mapDistance[E_LEFT | E_DOWN] = Diagonal_cost;
	m_mapDistance[E_RIGHT | E_DOWN] = Diagonal_cost;

	FILE* fp = NULL;
	fopen_s(&fp, "astar.txt", "r");

	int nIndex = 0;
	while (true)
	{
		char c = fgetc(fp);
		if (feof(fp)) break;
		if (c < 33) continue;
		cNode* pNode = new cNode;
		pNode->Setup(nIndex++, mapNodeType[c]);
		m_vecNode.push_back(pNode);
	}
	fclose(fp);

	FindPath();

}

void cAstarMainGame::Render(HDC hdc)
{
	for each (auto pNode in m_vecNode)
		pNode->Render(hdc);
}

void cAstarMainGame::FindPath()
{
	cNode* pStartNode = FindNode(cNode::E_START);
	cNode* pDestNode = FindNode(cNode::E_DEST);
	pStartNode->m_fG = 0.0f;
	pStartNode->m_fH = CalcHeuristic(pStartNode, pDestNode);
	pStartNode->m_fF = pStartNode->m_fG + pStartNode->m_fH;
	pStartNode->m_isInOpenList = true;
	m_pOpenList->Insert(pStartNode);

	while (true)
	{
		cNode* pMinFNode = m_pOpenList->MinFNode();
		if (pMinFNode == NULL)
		{
			//no openlist == no path
			MarkNodeType(pDestNode);
			break;
		}

		pMinFNode->m_isClosed = true;
		pMinFNode->m_isInOpenList = false;

		Extend(pMinFNode, pDestNode);
		if (pMinFNode == pDestNode)
		{
			//finding complete
			MarkNodeType(pDestNode);
			break;
		}
	}
}

cNode * cAstarMainGame::FindNode(cNode::eNodeType e)
{
	for each(auto pNode in m_vecNode)
	{
		if (pNode->m_eNodeType == e)
			return pNode;
	}
	return nullptr;
}

float cAstarMainGame::CalcHeuristic(cNode * pNode, cNode * pDestNode)
{
	return fabs(pNode->m_nRow - (float)pDestNode->m_nRow) + fabs(pNode->m_nCol - (float)pDestNode->m_nCol);
}

void cAstarMainGame::Extend(cNode * pExtendNode, cNode * pDestNode)
{
	for each (eDirection d in m_vecDirection)
	{
		cNode* pAdjNode = GetAdjNode(pExtendNode, d);

		if (!pAdjNode) continue;

		if (pAdjNode->m_isInOpenList)
		{
			if (pAdjNode->m_fG > pExtendNode->m_fG + m_mapDistance[d])
			{
				pAdjNode->m_pParent = pExtendNode;
				pAdjNode->m_fG = pExtendNode->m_fG + m_mapDistance[d];
				pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH;
				m_pOpenList->UpdateUpper(pAdjNode);
			}
		}
		else
		{
			pAdjNode->m_pParent = pExtendNode;
			pAdjNode->m_fG = pExtendNode->m_fG + m_mapDistance[d];
			pAdjNode->m_fH = CalcHeuristic(pAdjNode, pDestNode);
			pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH;
			pAdjNode->m_isInOpenList = true;
			m_pOpenList->Insert(pAdjNode);
		}
	}
}

cNode* cAstarMainGame::GetAdjNode(cNode * pNode, eDirection d)
{
	int nAdjRow = pNode->m_nRow;
	int nAdjCol = pNode->m_nCol;

	if (d & E_LEFT)
	{
		if (pNode->m_nCol == 0) return NULL;
		int nIndex = pNode->m_nRow * TILE_N + pNode->m_nCol - 1;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjCol -= 1;
	}

	if (d & E_RIGHT)
	{
		if (pNode->m_nCol == TILE_N - 1) return NULL;
		int nIndex = pNode->m_nRow * TILE_N + pNode->m_nCol + 1;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjCol += 1;
	}

	if (d & E_UP)
	{
		if (pNode->m_nRow == 0) return NULL;
		int nIndex = (pNode->m_nRow - 1) * TILE_N + pNode->m_nCol;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjRow -= 1;
	}

	if (d & E_DOWN)
	{
		if (pNode->m_nRow == TILE_N - 1) return NULL;
		int nIndex = (pNode->m_nRow + 1) * TILE_N + pNode->m_nCol;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjRow += 1;
	}

	int nIndex = nAdjRow * TILE_N + nAdjCol;
	if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
	if (m_vecNode[nIndex]->m_isClosed) return NULL;
	return m_vecNode[nIndex];
}

void cAstarMainGame::MarkNodeType(cNode * pDestNode)
{
	for each (auto pNode in m_vecNode)
	{
		if (pNode->m_isClosed) pNode->m_eNodeType = cNode::E_CLOSE;
		if (pNode->m_isInOpenList) pNode->m_eNodeType = cNode::E_OPEN;
	}

	cNode* pTemp = pDestNode;
	while (pTemp)
	{
		pTemp->m_eNodeType = cNode::E_PATH;
		pTemp = pTemp->m_pParent;
	}
}
