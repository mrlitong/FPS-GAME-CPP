#include "RoleBase.h"
#include "Creature.h"
#include "interface.h"
#include "Action.h"
#include "Utils.h"
#include "NavigationFinder.h"
#include "Engine.h"
#include "World.h"
#include "Object.h"
#include "Avatar.h"

CRoleBase::CRoleBase(void)
{
	m_pCreature = NULL;
	m_pActionComplete = NULL;
	m_nOrceDirection = 0;
	m_nMoveing = 0;
	m_fMoveSpeed = 6.0f;
	m_nMoveToType = 0;
	m_nUpdateMove = 1;
}

CRoleBase::~CRoleBase(void)
{
	SAFE_DELETE(m_pActionComplete);
	SAFE_DELETE(m_pCreature);
	SAFE_DELETE(m_pPathFind);
}

//初始化角色
int CRoleBase::Init(int nRoleID, const char* strCharFile)
{
	if (m_pCreature)return 1;

	m_nMoveing = 1;
	m_nRoleID = nRoleID;
	m_pCreature = new CCreature();
	m_pActionComplete = MakeInterface(this, &CRoleBase::OnActionMsg);
	m_pCreature->SetActionComplete(m_pActionComplete);

	m_pCreature->Load(strCharFile);
	m_pCreature->SetupBody(1, 0);
	m_pCreature->SetPosition(vec3_zero);
	m_pCreature->SetDirection(vec3(0.0f, -1.0f, 0.0f));
	m_pCreature->SetRoleID(nRoleID);

	m_AngleYaw.SetDirection(vec3(0.0f, -1.0f, 0.0f));

	m_pPathFind = new CNavigationFinder();
	m_pPathFind->SetMode(CNavigationFinder::MODE_PATHFIND_STRAIGHT);

	return 1;
}

void CRoleBase::Update(float ifps)
{
	if (NULL == m_pCreature)return;

	if (m_nUpdateMove)
	{
		m_AngleYaw.Update(ifps);
		UpdateMove(ifps);
		m_pCreature->SetDirection(m_AngleYaw.GetForwardDirection(), m_nOrceDirection);
	}

	m_pCreature->Update();
}


int CRoleBase::OnActionMsg(void* pVoid)
{
	_ActionCallback* pInfo = (_ActionCallback*)pVoid;

	switch (pInfo->nMsgID)
	{
	case 0:
	{
		OnKeyFrame((_ActionCallback_KeyFrame*)pVoid);
	}break;
	case 1:
	{
		OnActionComplete((_ActionCallback_Complete*)pVoid);
	}break;
	}

	return 1;
}
//关键帧回调
void CRoleBase::OnKeyFrame(_ActionCallback_KeyFrame* pKeyInfo)
{

}//动作完成回调
void CRoleBase::OnActionComplete(_ActionCallback_Complete* pActInfo)
{
	if (NULL == m_pCreature)return;

	if (m_nMoveing)
	{
		m_pCreature->PlayAction("run");
	}
	else
	{
		m_pCreature->PlayAction("stand");
	}
}
CAction* CRoleBase::PlayAction(const char* szName, int nLoop, float fCorrectingTime /*= 1.0f*/)
{
	if (NULL == m_pCreature)return NULL;

	if (1 == m_pCreature->PlayAction(szName, nLoop, fCorrectingTime))
	{
		return m_pCreature->GetNowAction();
	}

	return NULL;
}
CAction* CRoleBase::OrceAction(const char* szName, int nLoop, float fCorrectingTime /*= 1.0f*/)
{
	if (NULL == m_pCreature)return NULL;

	if (1 == m_pCreature->OrceAction(szName, nLoop, fCorrectingTime))
	{
		return m_pCreature->GetNowAction();
	}

	return NULL;
}
