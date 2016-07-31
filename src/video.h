#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <SDL.h>

#define SCREEN_W		320
#define SCREEN_H		240
#define SCREEN_BPP		16
#define FPS			60

#if !defined(SCREEN_SCALE)
#define SCREEN_SCALE 2
#endif

typedef int surfaceId;

extern SDL_Surface *screen;
extern int blinkTimer;
extern int blinkTimerSlow;
extern int scale;

typedef struct video
{
	int (*frameLimiter)();
	void (*flipScreen)();
	void (*clearScreen)();
} video;

int initSDL();
void deinitSDL();
void updateScale();
Uint32 getColor(Uint8 r, Uint8 g, Uint8 b);
SDL_Surface *loadImage(char *fileName);
void clipImage(SDL_Rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles);
void drawImage(SDL_Surface *source, SDL_Rect *clip, SDL_Surface *destination, int x, int y);
void drawBackground(SDL_Surface *destination, Uint32 color);
void drawPoint(SDL_Surface *destination, int x, int y, Uint32 color);
int frameLimiterSDL();
void flipScreenSDL();
void clearScreenSDL();

#endif /* _VIDEO_H_ */
