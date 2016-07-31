#include "main.h"

#include <stdlib.h>
#include <time.h>
#include "fileio.h"
#include "font.h"
#include "input.h"
#include "states.h"
#include "video.h"

int init(const video *video)
{
	if (getConfigDir())
	{
		printf("Failed to retrieve config path.\n");
		return -1;
	}

	getConfig();
	getHiscore();

	if(initSDL())
	{
		return -1;
	}

	fontLoad(video, &gameFontShadow, "data/gfx/fontBlack.bmp", 6, 11, 1, 4, NULL);
	fontLoad(video, &gameFont, "data/gfx/font.bmp", 6, 11, 1, 4, &gameFontShadow);

	return 0;
}

void deinit(const video *video)
{
	fontUnload(video, &gameFont);
	fontUnload(video, &gameFontShadow);

	deinitSDL();
}

int main(int argc, char *argv[])
{
	const video video =
	{
		.getScreenId = getScreenIdSDL,
		.loadImage = loadImageSDL,
		.unloadImage = unloadImageSDL,
		.setAlpha = setAlphaSDL,
		.clipImage = clipImageSDL,
		.drawImage = drawImageSDL,
		.drawBackground = drawBackgroundSDL,
		.drawPoint = drawPointSDL,
		.fillRect = fillRectSDL,
		.frameLimiter = frameLimiterSDL,
		.flipScreen = flipScreenSDL,
		.clearScreen = clearScreenSDL,
	};
	int i;
	quit = 0;

	for(i = 1; i < argc; i++)
	{
		if((!strcmp(argv[i], "-help")) || (!strcmp(argv[i], "-h")))
		{
			printf("Homing Fever v%d.%d.%d, Copyright (c) 2016 Artur Rojek.\nThis program is MIT licensed.\n\n", PROGRAM_MAJOR_VERSION, PROGRAM_MINOR_VERSION, PROGRAM_PATCH_VERSION);
			printf("Options:\n");
			printf("-s or -scale [1-2]\t\tScales the screen resolution\n");
			printf("-v or -version\t\t\tDisplay the program version\n");
			printf("-h or -help\t\t\tDisplay this help screen\n");
			return 0;
		}
		if ((!strcmp(argv[i], "-version")) || (!strcmp(argv[i], "-v")))
		{
			printf("%d.%d.%d\n", PROGRAM_MAJOR_VERSION, PROGRAM_MINOR_VERSION, PROGRAM_PATCH_VERSION);
			return 0;
		}
		if ((!strcmp(argv[i], "-scale")) || (!strcmp(argv[i], "-s")))
		{
			if (argc - 1 < i + 1)
			{
				printf("Not enough arguments for %s.\n1 integer parameter required.\n", argv[i]);
				return 1;
			}
			else
			{
				scale = atoi(argv[i+1]);
				if (scale < 1 || scale > 2)
				{
					printf("Invalid value '%s' for %s.\nValue must be between [1-2].\n", argv[i+1], argv[i]);
					return 1;
				}
			}
		}
	}

	if(init(&video))
	{
		quit = 1;
	}

	srand(time(NULL));

	while(!quit)
	{
		if(!video.frameLimiter())
		{
			input();
			logic(&video);
			draw(&video);
		}
	}

/*	storeConfig();*/
	deinit(&video);

	return 0;
}
