#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "video.h"

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))
#define MOD(a,b)	((((a)%(b))+(b))%(b))
#define FMODF(a,b)	(fmodf(fmodf((a), (b))+(b), (b)))
#define PI		3.14159265
#define SINE_STEPS	360

extern const float sineTable[];

typedef struct listElement
{
	struct listElement *next;
	void *item;
} listElement;

listElement *listElementPrepend(listElement *head);
listElement *listElementDelete(listElement *head, listElement *toDelNode, void(*itemDel)(void *item, const void *data), const void *data);
listElement *listElementDeleteMatching(listElement *head, void(*itemDel)(void *item, const void *data), int(*pattern)(void *item), const void *data);
listElement *listElementDeleteAll(listElement *head, void(*itemDel)(void *item, const void *data), const void *data);
listElement *listElementGet(listElement *head, unsigned int id);
unsigned int listLength(listElement *head);

#endif /* _HELPERS_H_ */
