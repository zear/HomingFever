#include "video.h"

#include <SDL.h>
#include "debug.h"
#include "input.h"
#include "scaler.h"

SDL_Surface *screen;
SDL_Surface *screenScaled;
int screenScale;
int fullscreen;
Uint32 curTicks;
Uint32 lastTicks = 0;

#define MAX_SURFACES 2048

int surfaceTail = 0;
surfaceId screenId = -1;
SDL_Surface *surfaces[MAX_SURFACES];

surfaceId getScreenIdSDL()
{
	return screenId;
}

int initSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK))
	{
		return -1;
	}

	SDL_WM_SetCaption("Homing Fever", NULL);
	SDL_ShowCursor(SDL_DISABLE);

	updateScaleSDL();

	if(screen == NULL)
	{
		return -1;
	}

	if(SDL_NumJoysticks() > joyNum)
	{
		joyDevice = SDL_JoystickOpen(joyNum);
	}

	return 0;
}

void deinitSDL()
{
	if(joyDevice)
	{
		SDL_JoystickClose(joyDevice);
	}

	if (screenScale > 1)
	{
		SDL_FreeSurface(screen);
	}

	SDL_Quit();
}

void updateScaleSDL()
{
	if (screen != screenScaled)
	{
		SDL_FreeSurface(screen);
	}

	screenScaled = SDL_SetVideoMode(SCREEN_W * screenScale, SCREEN_H * screenScale, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF | (fullscreen ? SDL_FULLSCREEN : 0));
	screen = screenScale > 1 ? SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_W, SCREEN_H, SCREEN_BPP, 0, 0, 0, 0) : screenScaled;

	surfaces[surfaceTail] = screen;
	screenId = surfaceTail;
	surfaceTail++;
}

Uint32 getColor(Uint8 r, Uint8 g, Uint8 b)
{
	return SDL_MapRGB(screen->format, r, g, b);
}

void unloadImageSDL(surfaceId id)
{
	SDL_FreeSurface(surfaces[id]);
}

void setAlphaSDL(surfaceId id, Uint8 alpha)
{
	SDL_SetAlpha(surfaces[id], SDL_SRCALPHA, alpha);
}

surfaceId loadImageSDL(char *fileName)
{
	SDL_Surface *loadedImage;
	SDL_Surface *optimizedImage;
	Uint32 colorKey;

	if (surfaceTail >= MAX_SURFACES)
	{
		fprintf(stderr, "ERROR: Maximum images loaded.");
		return -1;
	}

	if (!fileName)
	{
		fprintf(stderr, "ERROR: Filename is empty.");
		return -1;
	}

	loadedImage = SDL_LoadBMP(fileName);

	if (!loadedImage)
	{
		fprintf(stderr, "ERROR: Failed to load image: %s\n", fileName);
		return -1;
	}

	optimizedImage = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, loadedImage->w, loadedImage->h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_BlitSurface(loadedImage, NULL, optimizedImage, NULL);
	SDL_FreeSurface(loadedImage);

	if (!optimizedImage)
	{
		fprintf(stderr, "ERROR: Failed to optimize image: %s\n", fileName);
		return -1;
	}

	colorKey = SDL_MapRGB(optimizedImage->format, 255, 0, 255); /* Set transparency to magenta. */
	SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorKey);

	surfaces[surfaceTail] = optimizedImage;
	surfaceTail++;
	return surfaceTail - 1;
}

void clipImageSDL(rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles)
{
	int i;
	int j;
	int k;
	int l;

	for(i = 0, k = 0; k < numOfTiles; i+= tileHeight)
	{
		for(j = 0, l = 0; l < rowLength; j+= tileWidth)
		{
			source[k].x = j;
			source[k].y = i;
			source[k].w = tileWidth;
			source[k].h = tileHeight;
			++k;
			++l;
		}
		l = 0;
	}
}

void drawImageSDL(surfaceId source, rect *clip, surfaceId destination, int x, int y)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	if (clip)
	{
		SDL_Rect clipSDL;
		clipSDL.x = clip->x;
		clipSDL.y = clip->y;
		clipSDL.w = clip->w;
		clipSDL.h = clip->h;
		SDL_BlitSurface(surfaces[source], &clipSDL, surfaces[destination], &offset);
	} else {
		SDL_BlitSurface(surfaces[source], NULL, surfaces[destination], &offset);
	}
}

void drawBackgroundSDL(surfaceId destination, Uint32 color)
{
	SDL_FillRect(surfaces[destination], NULL, color);
}

void drawPointSDL(surfaceId destination, int x, int y, Uint32 color)
{
	SDL_Rect r;

	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = 1;

	SDL_FillRect(surfaces[destination], &r, color);
}


void fillRectSDL(surfaceId destination, rect *rect, Uint8 r, Uint8 g, Uint8 b)
{
	if (rect)
	{
		SDL_Rect clip;
		clip.x = rect->x;
		clip.y = rect->y;
		clip.w = rect->w;
		clip.h = rect->h;
		SDL_FillRect(surfaces[destination], &clip, SDL_MapRGB(surfaces[destination]->format, r, g, b));
	} else {
		SDL_FillRect(surfaces[destination], NULL, SDL_MapRGB(surfaces[destination]->format, r, g, b));
	}
}

int frameLimiterSDL()
{
	int t;

#if defined(NO_FRAMELIMIT)
	return 0;
#endif

	curTicks = SDL_GetTicks();
	t = curTicks - lastTicks;

	if(t >= 1000/FPS)
	{
		lastTicks = curTicks;
		return 0;
	}

	SDL_Delay(1);
	return 1;
}

void flipScreenSDL()
{
	switch (screenScale)
	{
		case 1:
		break;
		case 2:
			upscale2((uint32_t *)screenScaled->pixels, (uint32_t *)screen->pixels);
		break;
	}

	SDL_Flip(screenScaled);

	if (debugSlowMotion)
	{
		SDL_Delay(250);
	}
}

void clearScreenSDL()
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
}
