/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_PLAYER_H
#define GAME_SERVER_PLAYER_H

// this include should perhaps be removed
#include "entities/character.h"
#include "gamecontext.h"
#include <game/server/classes.h>

// player object
class CPlayer
{
	MACRO_ALLOC_POOL_ID()

public:
	CPlayer(CGameContext *pGameServer, int ClientID, int Team);
	~CPlayer();

	void Init(int CID);

	void TryRespawn();
	void Respawn();
	void SetTeam(int Team, bool DoChatMsg=true);
	int GetTeam() const { return m_Team; };
	int GetCID() const { return m_ClientID; };

	void Tick();
	void PostTick();
	void Snap(int SnappingClient);

	void FakeSnap(int SnappingClient);
	
	void OnDirectInput(CNetObj_PlayerInput *NewInput);
	void OnPredictedInput(CNetObj_PlayerInput *NewInput);
	void OnDisconnect(const char *pReason);

	void KillCharacter(int Weapon = WEAPON_GAME);
	CCharacter *GetCharacter();

	int GetClass();
	void SetClassSkin(int newClass, int State = 0);
	void SetClass(int newClass);
	int GetOldClass();
	void SetOldClass(int oldClass);
	bool IsZombie() const;
	bool IsHuman() const;
	bool IsSpectator() const;
	void StartInfection(bool force = false);
	bool IsKnownClass(int c);

	const char* GetLanguage();
	void SetLanguage(const char* pLanguage);

	//---------------------------------------------------------
	// this is used for snapping so we know how we can clip the view for the player
	vec2 m_ViewPos;

	// states if the client is chatting, accessing a menu etc.
	int m_PlayerFlags;

	// used for snapping to just update latency if the scoreboard is active
	int m_aActLatency[MAX_CLIENTS];

	// used for spectator mode
	int m_SpectatorID;

	//
	int m_Vote;
	int m_VotePos;
	//
	int m_LastVoteCall;
	int m_LastVoteTry;
	int m_LastChat;
	int m_LastSetTeam;
	int m_LastSetSpectatorMode;
	int m_LastChangeInfo;
	int m_LastEmote;
	int m_LastKill;

	// TODO: clean this up
	struct
	{
		char m_SkinName[64];
		int m_UseCustomColor;
		int m_ColorBody;
		int m_ColorFeet;
	} m_TeeInfos;

	int m_RespawnTick;
	int m_DieTick;
	int m_Score;
	int m_ScoreStartTick;
	bool m_ForceBalanced;
	int m_LastActionTick;
	int m_TeamChangeTick;
	struct
	{
		int m_TargetX;
		int m_TargetY;
	} m_LatestActivity;

	// network latency calculations
	struct
	{
		int m_Accum;
		int m_AccumMin;
		int m_AccumMax;
		int m_Avg;
		int m_Min;
		int m_Max;
	} m_Latency;

	int m_Authed;

	bool m_knownClass[NB_PLAYERCLASS];

	bool m_IsReady;
	bool m_IsInGame;

	int m_MapMenuItem;
	int m_LastHumanClasses[2];
	int m_InfectionTick;

	int MapMenu() { return (m_Team != TEAM_SPECTATORS) ? m_MapMenu : 0; };
	void OpenMapMenu(int Menu);
	void CloseMapMenu();
	bool MapMenuClickable();
private:
	CCharacter *m_pCharacter;
	CGameContext *m_pGameServer;

	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const;

	//
	bool m_Spawning;
	int m_ClientID;
	int m_Team;

	char m_aLanguage[16];

	private:
	CTuningParams m_PrevTuningParams;
	CTuningParams m_NextTuningParams;

	void HandleTuningParams(); //This function will send the new parameters if needed

	int m_class;
	int m_MapMenu;
	int m_MapMenuTick;

public:
	CTuningParams* GetNextTuningParams() { return &m_NextTuningParams; };
};

enum
{
	PLAYERITER_ALL=0x0,
	
	PLAYERITER_COND_READY=0x1,
	PLAYERITER_COND_SPEC=0x2,
	PLAYERITER_COND_NOSPEC=0x4,
	
	PLAYERITER_INGAME = PLAYERITER_COND_READY | PLAYERITER_COND_NOSPEC,
	PLAYERITER_SPECTATORS = PLAYERITER_COND_READY | PLAYERITER_COND_SPEC,
};

template<int FLAGS>
class CPlayerIterator
{
private:
	CPlayer** m_ppPlayers;
	int m_ClientID;
	
public:
	
	CPlayerIterator(CPlayer** ppPlayers) :
		m_ppPlayers(ppPlayers)
	{
		Reset();
	}
	
	inline bool Next()
	{
		for(m_ClientID = m_ClientID+1; m_ClientID<MAX_CLIENTS; m_ClientID++)
		{
			CPlayer* pPlayer = Player();
			
			if(!pPlayer) continue;
			if((FLAGS & PLAYERITER_COND_READY) && (!pPlayer->m_IsInGame)) continue;
			if((FLAGS & PLAYERITER_COND_NOSPEC) && (pPlayer->GetTeam() == TEAM_SPECTATORS)) continue;
			if((FLAGS & PLAYERITER_COND_SPEC) && (pPlayer->GetTeam() != TEAM_SPECTATORS)) continue;
			
			return true;
		}
		
		return false;
	}
	
	inline void Reset() { m_ClientID = -1; }
	
	inline CPlayer* Player() { return m_ppPlayers[m_ClientID]; }
	inline int ClientID() { return m_ClientID; }
};

#endif
