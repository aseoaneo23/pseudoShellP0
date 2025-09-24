#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void imprimirPrompt();

void leerEntrada(char *comando);

bool procesarEntrada(char *comando);

int TrocearCadena(char *cadena, char *trozos[]);

void authors(char *mod);

void getShellPid(char *mod);

void changeDir(char *path);

void printCurrentDir();

void infosys(char *mod);

void helpCmd(char *mod);

void dateCmd(char *mod);

void hourCmd(char *mod);

