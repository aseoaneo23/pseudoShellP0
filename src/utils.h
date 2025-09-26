#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include "lista_comandos.h"
#include "lista.h"

typedef enum
{
    N,
    LAST_N,
    COUNT,
    CLEAR,
    NO_MOD
} MOD;

typedef struct
{
    const char *flag;
    MOD modifier;

} ModMap;


void imprimirPrompt();

void leerEntrada(char *comando);

bool procesarEntrada(char *comando, tList *historical);

int TrocearCadena(char *cadena, char *trozos[]);

void authors(char *mod);

void getShellPid(char *mod);

void changeDir(char *path);

void printCurrentDir();

void createList();

bool updateHistorical(tList *historical, char *command);

void printHistorical(tList historical);

void customHistoricalPrint(MOD type, char *mod, tList historical);

void infosys(char *mod);

void helpCmd(char *mod);

void dateCmd(char *mod);

void closeCmd(char *mod);

void dupCmd(char *mod);

void ListaFichAbiertos(void);

void EliminarFichAbiertos(int fd);

char *NameFicheroDescriptor(int fd);

void AnadirFicherosAbiertos(int fd, const char *nombre, int flags);

void cleanListFromMemory(tList *L);

MOD identifyModifier(char *mod);

void manageHistoricalWMods(char *mod, tList *historical);


