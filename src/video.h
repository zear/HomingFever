#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <stdint.h>

#define SCREEN_W		320
#define SCREEN_H		240
#define SCREEN_BPP		16
#define FPS			60

#if !defined(SCREEN_SCALE)
#define SCREEN_SCALE 2
#endif

typedef int surfaceId;

extern int screenScale;
extern int fullscreen;
extern int blinkTimer;
extern int blinkTimerSlow;

typedef struct rect
{
	int x;
	int y;
	int w;
	int h;
} rect;

typedef struct video
{
	surfaceId (*getScreenId)();
	surfaceId (*loadImage)(char *fileName);
	void (*unloadImage)(surfaceId);
	void (*setAlpha)(surfaceId, uint8_t alpha);
	void (*clipImage)(rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles);
	void (*drawImage)(surfaceId source, rect *clip, surfaceId destination, int x, int y);
	void (*drawBackground)(surfaceId destination, uint32_t color);
	void (*drawPoint)(surfaceId destination, int x, int y, uint32_t color);
	void (*fillRect)(surfaceId destination, rect *rect, uint8_t r, uint8_t g, uint8_t b);
	int (*frameLimiter)();
	void (*flipScreen)();
	void (*clearScreen)();
} video;

int initSDL();
void deinitSDL();
void updateScaleSDL();
uint32_t getColor(uint8_t r, uint8_t g, uint8_t b);
surfaceId getScreenIdSDL();
surfaceId loadImageSDL(char *fileName);
void unloadImageSDL(surfaceId id);
void setAlphaSDL(surfaceId id, uint8_t alpha);
void clipImageSDL(rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles);
void drawImageSDL(surfaceId source, rect *clip, surfaceId destination, int x, int y);
void drawBackgroundSDL(surfaceId destination, uint32_t color);
void drawPointSDL(surfaceId destination, int x, int y, uint32_t color);
void fillRectSDL(surfaceId destination, rect *rect, uint8_t r, uint8_t g, uint8_t b);
int frameLimiterSDL();
void flipScreenSDL();
void clearScreenSDL();

#endif /* _VIDEO_H_ */
