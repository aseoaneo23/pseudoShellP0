#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include "lista_comandos.h"
#include "lista_osoverde.h"

void imprimirPrompt();

void leerEntrada(char *comando);

bool procesarEntrada(char *comando, tList *historical);

int TrocearCadena(char *cadena, char *trozos[]);

void authors(char *mod);

void getShellPid(char *mod);

void changeDir(char *path);

void printCurrentDir();

void createList();

void updateHistorical(tList *historical, char *command);

void printHistorical(tList historical);

void infosys(char *mod);

void helpCmd(char *mod);

void dateCmd(char *mod);

void hourCmd(char *mod);

