#pragma once
class iMap;

class cCharacter
{
public:
	cCharacter();

protected:
	D3DXVECTOR3		m_vPosition;
	D3DXVECTOR3		m_vRotation;
	D3DXVECTOR3		m_vDirection;

	D3DXMATRIXA16	m_matWorld;

	bool m_IsMoving;
	bool m_IsJumping;

	int m_jumpTime;
	float m_fGravity;

	iMap* m_pMap;
	//vector<cGroup*> m_vecGroup;
	//vector<cGroup*> m_vecGroup2;

public:
	virtual ~cCharacter();

	virtual void Setup();
	virtual void Update(iMap* pMap);
	virtual void Render();

	virtual D3DXVECTOR3& GetPosition();
	
	//float FloorIntersect();
	//float WallIntersect(bool isFront);
};

