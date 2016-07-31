#ifndef _STATES_H_
#define _STATES_H_

#include "video.h"

typedef enum State
{
	STATE_NONE = 0,
	STATE_TITLE,
	STATE_GAME,
	STATE_HISCORE
} State;

extern int quit;

extern State programStateActive;
extern State programStateNew;

void checkState();
void logic();
void draw(const video *);

#endif /* _STATES_H_ */
