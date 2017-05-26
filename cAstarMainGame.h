#pragma once

#include "cNode.h"
class cOpenList;

class cAstarMainGame
{
public:
	cAstarMainGame();
	~cAstarMainGame();

private:
	std::vector<cNode*> m_vecNode;
	cOpenList* m_pOpenList;

	enum
	{
		E_LEFT = 1 << 0,
		E_RIGHT = 1 << 1,
		E_UP = 1<< 2,
		E_DOWN = 1<< 3,
	};

	typedef int eDirection;
	std::vector<eDirection> m_vecDirection;
	std::map<eDirection, float> m_mapDistance;

public:
	void Setup();
	void Render(HDC hdc);
	void FindPath();
	cNode* FindNode(cNode::eNodeType e);
	float CalcHeuristic(cNode* pNode, cNode* pDestNode);
	void Extend(cNode* pExtendNode, cNode* pDestNode);
	cNode* GetAdjNode(cNode* pNode, eDirection d);
	void MarkNodeType(cNode* pDestNode);

};

