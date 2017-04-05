#include "StarControl.h"
#include "ObjectParticles.h"
#include "Engine.h"
#include "Game.h"
#include "Object.h"
#include "Player.h"
#include "Common.h"
#include "App.h"

CStarControl::CStarControl(void)
{
	m_pStarNormal = NULL;
	m_pStarClick = NULL;
	m_fViewDistance = 0.5f;
	m_nClickState = 0;
	m_fClickScale = 1.0f;
	Init();
}

CStarControl::~CStarControl(void)
{
	g_Engine.pGame->RemoveNode(m_pStarNormal);
	g_Engine.pGame->RemoveNode(m_pStarClick);
}

int CStarControl::Init()
{
	return Load("data/StarControl/Star_mesh.node", "data/StarControl/Star_click.node");
}

void CStarControl::Update(const vec3& vPos, const vec3& vDir)
{
	m_fClickScale += m_nClickState * g_Engine.pGame->GetIFps() * 5.5f;
	if (m_fClickScale <= 0.8f)
	{
		m_nClickState = 1;
	}
	else if (m_fClickScale >= 1.0f)
	{
		m_fClickScale = 1.0f;
		m_nClickState = 0;
	}

}
