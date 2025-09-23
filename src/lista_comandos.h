#ifndef LISTA_COMANDOS_H
#define LISTA_COMANDOS_H

#include <stdlib.h>
#include <stdbool.h>

#define LNULL NULL
#define TOPE 500

typedef struct {
    char text[TOPE];
} tItemL;

typedef struct tNodo* tPosL;

typedef struct tNodo{
    tItemL info;
    tPosL nextnodo;
}tNodo;

typedef tPosL tList;

void createEmptyList(tList *L);
bool isEmptyList(tList L);
tPosL first(tList L);
tPosL last(tList L);
tPosL next(tPosL p, tList L);
tPosL previous(tPosL p, tList L);
bool insertItem(tItemL d, tPosL p, tList *L);
void deleteAtPosition(tPosL p, tList *L);

#endif
