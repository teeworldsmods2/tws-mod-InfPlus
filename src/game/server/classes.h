#ifndef GAME_SERVER_CLASSES_H
#define GAME_SERVER_CLASSES_H

enum
{
	HUMANCLASS_NONE = 0,
	
	START_HUMANCLASS,
    HUMANCLASS_TRUEMAN,
	HUMANCLASS_MEDIC,
    HUMANCLASS_HERO,
    HUMANCLASS_LOOPER,
	END_HUMANCLASS,
	
	START_INFECTEDCLASS,
	ZOMBIECLASS_ZABY,
	ZOMBIECLASS_BOOMER,
	ZOMBIECLASS_HUNTER,
	ZOMBIECLASS_BAT,
	ZOMBIECLASS_SPIDER,
	END_INFECTEDCLASS,
	
	NB_PLAYERCLASS,
	NB_HUMANCLASS = END_HUMANCLASS - START_HUMANCLASS - 1,
	NB_INFECTEDCLASS = END_INFECTEDCLASS - START_INFECTEDCLASS - 1,
};

#endif
