#include "states.h"

#include "game.h"
#include "title.h"
#include "video.h"

int quit;

State programStateActive = STATE_NONE;
State programStateNew = STATE_TITLE;

void checkState(const video *video)
{
	if (programStateActive != programStateNew)
	{
		/* Unload current state. */
		switch (programStateActive)
		{
			case STATE_TITLE:
				titleUnload(video);
			break;
			case STATE_GAME:
				gameUnload(video);
			break;

			default:
			break;
		}
		/* Load new state. */
		switch (programStateNew)
		{
			case STATE_TITLE:
				titleLoad(video);
			break;
			case STATE_GAME:
				gameLoad(video);
			break;

			default:
			break;
		}

		programStateActive = programStateNew;
	}
}

void logic(const video *video)
{
	checkState(video);

	switch (programStateActive)
	{
		case STATE_TITLE:
			titleLogic();
		break;
		case STATE_GAME:
			gameLogic(video);
		break;

		default:
		break;
	}
}

void draw(const video *video)
{
	video->clearScreen();

	switch (programStateActive)
	{
		case STATE_TITLE:
			titleDraw(video);
		break;
		case STATE_GAME:
			gameDraw(video);
		break;

		default:
		break;
	}

	video->flipScreen();
}
