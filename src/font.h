#ifndef _FONT_H_
#define _FONT_H_

#include "tileset.h"
#include "video.h"

#define FONT_SHADOW_OFFSET_X	1
#define FONT_SHADOW_OFFSET_Y	1

typedef struct font
{
	tileset tiles;
	int w;
	int h;
	int tracking;
	int leading;
	struct font *shadow;
} font;

typedef enum shadowType
{
	SHADOW_NONE = 0,
	SHADOW_DROP,
	SHADOW_OUTLINE
} shadowType;

typedef enum alphaType
{
	ALPHA_TRANSPARENT = 0,
	ALPHA_TRANSLUCENT = 128,
	ALPHA_OPAQUE = 255
} alphaType;


extern font gameFont;
extern font gameFontShadow;

void fontLoad(const video *, font *fontObj, char *filename, int glyphWidth, int glyphHeight, int tracking, int leading, font *shadow);
void fontUnload(const video *, font *fontObj);
void dTextCentered(const video *, font *fontObj, char *string, int y, int alpha, shadowType withShadow);
void dText(const video *, font *fontObj, char *string, int x, int y, int alpha, shadowType withShadow);
void dTextEmerging(const video *, font *fontObj, char *string, int x, int y, int step, int alpha, shadowType withShadow);

#endif /* _FONT_H_ */
