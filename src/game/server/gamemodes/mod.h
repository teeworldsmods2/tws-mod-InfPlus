/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_MOD_H
#define GAME_SERVER_GAMEMODES_MOD_H
#include <game/server/gamecontroller.h>
#include <game/server/gameworld.h>
#include <game/server/classes.h>

class CGameControllerMOD : public IGameController
{
public:
	CGameControllerMOD(class CGameContext *pGameServer);
	virtual void Tick();
	virtual void Snap(int SnappingClient);
	virtual int ChooseHumanClass(const CPlayer *pPlayer) const;
	virtual int ChooseInfectedClass() const;
	virtual bool IsChoosableClass(int PlayerClass);
};
#endif
