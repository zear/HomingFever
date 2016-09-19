#ifndef _TILESET_H_
#define _TILESET_H_

#include "video.h"

typedef struct tileset
{
	surfaceId image;
	rect *clip;
	int rowLen;
	int length;
} tileset;

void tilesetLoad(const video *, tileset *tSet, char *fileName, int width, int height, int rowLen, int length);
void tilesetUnload(const video *, tileset *tSet);

#endif /* _TILESET_H_ */
