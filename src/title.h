#ifndef _TITLE_H_
#define _TITLE_H_

#define TEXT_PAGES	5
#define TEXT_LINES	7
#define TEXT_LINE_LEN	100
#define FADE_OUT_TIME	30

#include "video.h"

void titleUnload(const video *);
void titleLoad(const video *);
void titleLogic();
void titleDraw(const video *);

#endif /* _TITLE_H_ */
