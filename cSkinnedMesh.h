#pragma once
class cSkinnedMesh
{
public:
	cSkinnedMesh();
	~cSkinnedMesh();

protected:
	LPD3DXFRAME m_pRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;	//모든 애니메이션 정보
public:
	void Setup(char* szFolder, char* szFile);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);
	void Render(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);
	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);

	//animation
	//void SetAnimationIndex(int nIndex);
	//m_pAnimController->GetNumAnimationSets();
	//LPD3DXANIMATIONSET ...
	//m_pAnimController->GetAnimationSet(index, ...);
	//m_pAnimController->SetTrackAnimationSet(0, ...);
	//마우스 우클릭하면 애니메이션 변경하는 코드
};

