#pragma once
class cGroup;

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

	vector<cGroup*> m_vecGroup;
	vector<cGroup*> m_vecGroup2;

public:
	virtual ~cCharacter();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual D3DXVECTOR3& GetPosition();
	
	float FloorIntersect();
};

