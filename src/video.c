#include "video.h"

#include <SDL.h>
#include "debug.h"
#include "input.h"
#include "scaler.h"

SDL_Surface *screen;
SDL_Surface *screenScaled;
int screenScale;
#if defined(PORT_RG353P)
int fullscreen = 1;
#else
int fullscreen;
#endif
Uint32 curTicks;
Uint32 lastTicks = 0;

int initSDL()
{
#if defined(PORT_RG353P)
	char driver[128];
	fprintf(stderr, "[video] SDL video initialization starting\n");
#endif
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
		return -1;
	}

#if defined(PORT_RG353P)
	fprintf(stderr, "[video] SDL video initialized; driver=%s\n",
		SDL_VideoDriverName(driver, sizeof(driver)) ? driver : "unknown");
#endif
	SDL_WM_SetCaption("Homing Fever", NULL);
	SDL_ShowCursor(SDL_DISABLE);

#if defined(PORT_RG353P)
	fprintf(stderr, "[video] Setting mode %dx%d, fullscreen=%d\n",
		SCREEN_W * screenScale, SCREEN_H * screenScale, fullscreen);
#endif
	updateScale();

	if(screen == NULL || screenScaled == NULL)
	{
		fprintf(stderr, "SDL video mode failed: %s\n", SDL_GetError());
		return -1;
	}

#if defined(PORT_RG353P)
	fprintf(stderr, "[video] Mode set; bpp=%d pitch=%d flags=0x%lx\n",
		screenScaled->format->BitsPerPixel, screenScaled->pitch,
		(unsigned long)screenScaled->flags);
	fprintf(stderr, "[input] Opening joystick %d\n", joyNum);
#endif
	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK))
	{
		fprintf(stderr, "SDL joystick initialization failed: %s\n", SDL_GetError());
		return -1;
	}
	if(SDL_NumJoysticks() > joyNum)
	{
		joyDevice = SDL_JoystickOpen(joyNum);
	}

#if defined(PORT_RG353P)
	fprintf(stderr, "[input] Joystick %s\n", joyDevice ? "opened" : "unavailable");
#endif
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

void updateScale()
{
	if (screen != screenScaled)
	{
		SDL_FreeSurface(screen);
	}

	screenScaled = SDL_SetVideoMode(SCREEN_W * screenScale, SCREEN_H * screenScale, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF | (fullscreen ? SDL_FULLSCREEN : 0));
	screen = screenScale > 1 ? SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_W, SCREEN_H, SCREEN_BPP, 0, 0, 0, 0) : screenScaled;
}

Uint32 getColor(Uint8 r, Uint8 g, Uint8 b)
{
	return SDL_MapRGB(screen->format, r, g, b);
}

SDL_Surface *loadImage(char *fileName)
{
	SDL_Surface *loadedImage;
	SDL_Surface *optimizedImage;
	Uint32 colorKey;

	if (!fileName)
	{
		fprintf(stderr, "ERROR: Filename is empty.");
		return NULL;
	}

	loadedImage = SDL_LoadBMP(fileName);

	if (!loadedImage)
	{
		fprintf(stderr, "ERROR: Failed to load image: %s\n", fileName);
		return NULL;
	}

	optimizedImage = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF | (fullscreen ? SDL_FULLSCREEN : 0), loadedImage->w, loadedImage->h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_BlitSurface(loadedImage, NULL, optimizedImage, NULL);
	SDL_FreeSurface(loadedImage);

	if (!optimizedImage)
	{
		fprintf(stderr, "ERROR: Failed to optimize image: %s\n", fileName);
		return NULL;
	}

	colorKey = SDL_MapRGB(optimizedImage->format, 255, 0, 255); /* Set transparency to magenta. */
	SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorKey);

	return optimizedImage;
}

void clipImage(SDL_Rect *source, int tileWidth, int tileHeight, int rowLength, int numOfTiles)
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

void drawImage(SDL_Surface *source, SDL_Rect *clip, SDL_Surface *destination, int x, int y)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

void drawBackground(SDL_Surface *destination, Uint32 color)
{
	SDL_FillRect(destination, NULL, color);
}

void drawPoint(SDL_Surface *destination, int x, int y, Uint32 color)
{
	SDL_Rect r;

	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = 1;

	SDL_FillRect(destination, &r, color);
}

int frameLimiter()
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

void flipScreen()
{
#if defined(PORT_RG353P)
	static int firstFrame = 1;
	if (firstFrame)
		fprintf(stderr, "[video] Presenting first frame\n");
#endif
	switch (screenScale)
	{
		case 1:
		break;
		case 2:
			upscale2((uint32_t *)screenScaled->pixels, (uint32_t *)screen->pixels);
		break;
	}

	SDL_Flip(screenScaled);
#if defined(PORT_RG353P)
	if (firstFrame)
	{
		fprintf(stderr, "[video] First frame presented\n");
		firstFrame = 0;
	}
#endif

	if (debugSlowMotion)
	{
		SDL_Delay(250);
	}
}

void clearScreen()
{
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
}
