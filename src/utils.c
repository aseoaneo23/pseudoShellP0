#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

#define MAX_HOST_LENGTH 256

void imprimirPrompt()
{
    char *userName = getlogin();
    char host[MAX_HOST_LENGTH];


    if (gethostname(host,sizeof(host)) != 0){
        strcpy(host, "maquina_desconocida");
    }
    else if (userName == NULL){
        userName = "desconocido";
    }
    printf("%s@%s~:", userName, host);
}

void leerEntrada(char *comando)
{
    fgets(comando, 100, stdin);
}

bool procesarEntrada(char *comando)
{
    bool terminado = false;

    char **trozos = malloc(10 * sizeof(char *));
    int num_trozos = TrocearCadena(comando, trozos);

    if (num_trozos == 0)
    {
        free(trozos);
        return false;
    }

    if (strcmp(trozos[0], "authors") == 0)
        authors(trozos[1]);
    else if (strcmp(trozos[0], "getpid") ==0 )
        getShellPid(trozos[1]);
    else if (strcmp(trozos[0], "chdir") == 0)
        changeDir(trozos[1]);
    else if (strcmp(trozos[0], "getcwd") == 0)
        printCurrentDir();
    else if (strcmp(trozos[0], "exit") == 0 || strcmp(trozos[0], "quit") == 0 || strcmp(trozos[0], "bye") == 0)
        terminado = true;

    free(trozos);
    return terminado;
}

int TrocearCadena(char *cadena, char *trozos[])
{
    int i = 1;
    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

// Comandos p0

void authors(char *mod)
{
    if (mod == NULL)
    {
        printf("Antonio Seoane: antonio.seoane.deois@udc.es\n");
        printf("Sofía Oubiña: sofía.oubiña.@udc.es\n");
    }
    else if (strcmp(mod, "-l") == 0)
    {
        printf("antonio.seoane.deois@udc.es\n");
        printf("sofía.oubiña.@udc.es\n");
    }
    else if (strcmp(mod, "-n") == 0)
    {
        printf("Antonio Seoane\n");
        printf("Sofía Oubiña\n");
    }
}

void getShellPid(char *mod) {
    if (mod != NULL && strcmp(mod, "-p") == 0) {
        printf("Pid del padre del shell: %d\n", getppid());
        return;
    }
    printf("Pid de shell: %d\n", getpid());
}

void printCurrentDir() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

void changeDir(char *path) {
    if (path == NULL) {
        printCurrentDir();
        return;
    }
    chdir(path);    

}

