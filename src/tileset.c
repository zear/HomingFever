#include "tileset.h"

#include <stdio.h>
#include <stdlib.h>
#include "video.h"

void tilesetLoad(const video *video, tileset *tSet, char *fileName, int width, int height, int rowLen, int length)
{
	if (!tSet)
	{
		return;
	}

	if (!fileName)
	{
		return;
	}

	tSet->image = video->loadImage(fileName);
	if (!tSet->image)
	{
		fprintf(stderr, "ERROR: Failed to load file: %s\n", fileName);
		return;
	}

	if (!tSet->clip)
	{
		tSet->clip = malloc(sizeof(rect) * length);
		if (!tSet->clip)
		{
			fprintf(stderr, "ERROR: Not enough memory for allocation.\n");
			return;
		}
	}
	video->clipImage(tSet->clip, width, height, rowLen, length);
	tSet->rowLen = rowLen;
	tSet->length = length;
}

void tilesetUnload(const video *video, tileset *tSet)
{
	if (!tSet)
	{
		return;
	}

	video->unloadImage(tSet->image);
	tSet->image = -1;
	free(tSet->clip);
	tSet->clip = NULL;
}
