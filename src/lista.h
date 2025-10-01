#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

// #include "types.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef char* tItemL;

#define LNULL NULL

typedef struct tNode *tPosL;

struct tNode {
    tItemL data;
    tPosL next;
};

typedef tPosL tList;

void createEmptyList(tList *L);

bool isEmptyList(tList L);

tPosL first(tList L);

tPosL last(tList L);

tPosL next(tPosL p, tList L);

tPosL previous(tPosL p, tList L);

void updateItem(tItemL d, tPosL p, tList *L);

bool insertItem(tItemL d, tPosL p, tList *L);

void deleteAtPosition(tPosL p, tList *L);

tItemL getItem(tPosL p, tList L);

tPosL findItem(tItemL c, tList L);

void cleanMemory(tList *L);

int countItems (tList *L);

tPosL findItemByOffset(tPosL initPos, int offset, tList list);

#endif