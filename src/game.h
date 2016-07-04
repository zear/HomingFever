#ifndef _GAME_H_
#define _GAME_H_

#include "helpers.h"
#include "objects.h"

#define LEVEL_W			1600
#define LEVEL_H			1600

#define PLAYER_SPEED		2
#define PLAYER_ROTATION		4
#define PLAYER_PENALTY_TIME	(60*8)

extern listElement *objListHead;
extern object *playerObj;
extern tileset marker;
extern int gameTime;
extern int bestTime;
extern int playerLastAngle;
extern int playerPenaltyTimer;

void gameUnload();
void gameLoad();
void gameLogic();
void gameDraw();

#endif /* _GAME_H_ */
