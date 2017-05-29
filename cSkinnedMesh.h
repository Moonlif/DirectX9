#pragma once
class cSkinnedMesh
{
public:
	cSkinnedMesh();
	~cSkinnedMesh();

protected:
	LPD3DXFRAME m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;	//모든 애니메이션 정보

	float m_fBlendTime;
	float m_fPassedBlendTime;
	bool m_isAnimBlend;

public:
	void Setup(char* szFolder, char* szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);

	//animation
	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nIndex);
};

