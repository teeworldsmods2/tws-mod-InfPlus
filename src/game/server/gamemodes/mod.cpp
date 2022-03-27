/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include "mod.h"

#include <game/mapitems.h>
#include <time.h>
#include <iostream>
#include <game/server/player.h>
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>
#include <game/mapitems.h>
#include <engine/server/mapconverter.h>

CGameControllerMOD::CGameControllerMOD(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "iInfcPlus";

	//m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode
}

void CGameControllerMOD::Tick()
{
	// this is the main part of the gamemode, this function is run every tick

	IGameController::Tick();
}

void CGameControllerMOD::Snap(int SnappingClient)
{
	CNetObj_GameInfo *pGameInfoObj = (CNetObj_GameInfo *)Server()->SnapNewItem(NETOBJTYPE_GAMEINFO, 0, sizeof(CNetObj_GameInfo));
	if(!pGameInfoObj)
		return;

	pGameInfoObj->m_GameFlags = m_GameFlags;
	pGameInfoObj->m_GameStateFlags = 0;
	if(m_GameOverTick != -1)
		pGameInfoObj->m_GameStateFlags |= GAMESTATEFLAG_GAMEOVER;
	if(m_SuddenDeath)
		pGameInfoObj->m_GameStateFlags |= GAMESTATEFLAG_SUDDENDEATH;
	if(GameServer()->m_World.m_Paused)
		pGameInfoObj->m_GameStateFlags |= GAMESTATEFLAG_PAUSED;
	pGameInfoObj->m_RoundStartTick = m_RoundStartTick;
	pGameInfoObj->m_WarmupTimer = m_Warmup;

	pGameInfoObj->m_ScoreLimit = g_Config.m_SvScorelimit;
	pGameInfoObj->m_TimeLimit = g_Config.m_SvTimelimit;

	pGameInfoObj->m_RoundNum = (str_length(g_Config.m_SvMaprotation) && g_Config.m_SvRoundsPerMap) ? g_Config.m_SvRoundsPerMap : 0;
	pGameInfoObj->m_RoundCurrent = m_RoundCount+1;

	//Generate class mask
	int ClassMask = 0;
	{
		int Defender = 0;
		int Medic = 0;
		int Hero = 0;
		int Support = 0;
		int King = 0;
		
		CPlayerIterator<PLAYERITER_INGAME> Iter(GameServer()->m_apPlayers);
		while(Iter.Next())
		{
			switch(Iter.Player()->GetClass())
			{
				case HUMANCLASS_MEDIC:
					Medic++;
					break;
				case HUMANCLASS_HERO:
					Hero++;
					break;
				case HUMANCLASS_LOOPER:
					Defender++;
					break;
			}
		}
			ClassMask |= CMapConverter::MASK_DEFENDER;
			ClassMask |= CMapConverter::MASK_MEDIC;
			ClassMask |= CMapConverter::MASK_HERO;
			ClassMask |= CMapConverter::MASK_SUPPORT;
	}
	
	if(SnappingClient != -1)
	{
	if(GameServer()->m_apPlayers[SnappingClient])
	{
		int Page = -1;
		
		if(GameServer()->m_apPlayers[SnappingClient]->MapMenu() == 1)
		{
			int Item = GameServer()->m_apPlayers[SnappingClient]->m_MapMenuItem;
			Page = CMapConverter::TIMESHIFT_MENUCLASS + 3*((Item+1) + ClassMask*CMapConverter::TIMESHIFT_MENUCLASS_MASK) + 1;
		}
		
		if(Page >= 0)
		{
			double PageShift = static_cast<double>(Page * Server()->GetTimeShiftUnit())/1000.0f;
			double CycleShift = fmod(static_cast<double>(Server()->Tick() - pGameInfoObj->m_RoundStartTick)/Server()->TickSpeed(), Server()->GetTimeShiftUnit()/1000.0);
			int TimeShift = (PageShift + CycleShift)*Server()->TickSpeed();
			
			pGameInfoObj->m_RoundStartTick = Server()->Tick() - TimeShift;
			pGameInfoObj->m_TimeLimit += (TimeShift/Server()->TickSpeed())/60;
		}
	}
	}
}

bool CGameControllerMOD::IsChoosableClass(int PlayerClass)
{
	int nbDefender = 0;
	int nbMedic = 0;
	int nbHero = 0;
	int nbSupport = 0;
	int nbKing = 0;

	CPlayerIterator<PLAYERITER_INGAME> Iter(GameServer()->m_apPlayers);
	while(Iter.Next())
	{
		switch(Iter.Player()->GetClass())
		{
			case HUMANCLASS_MEDIC:
				nbMedic++;
				break;
			case HUMANCLASS_HERO:
				nbHero++;
				break;
			case HUMANCLASS_LOOPER:
				nbDefender++;
				break;
		}
	}
	
	
	return true;
}

int CGameControllerMOD::ChooseHumanClass(const CPlayer *pPlayer) const
{

}

int CGameControllerMOD::ChooseInfectedClass(const CPlayer *pPlayer) const
{

}