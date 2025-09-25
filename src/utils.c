#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>


#include "utils.h"
// #include "lista_comandos.h"
#include "lista_osoverde.h"

#define MAX_HOST_LENGTH 256
#define TOPE 500
/*
struct cmd {
    char *nombre;
    void (*func)(char **args);
};

struct cmd comandos[] = {
    {"authors", authors},
    {"getpid", getShellPid},
    {"chdir", changeDir},
    {"getcwd", printCurrentDir},
    {"historic", printHistorical},
    {NULL, NULL}

procesar_entrada(char *trozos[]) {
for int i = 0; comandos[i].nombre != NULL; i++) {
    if (!strcmp(trozos[0], comandos[i].nombre)}
        *comandos[i].func(trozos ++);
        return;
    }
    
Tener en cuenta que se están mandando todos los argumentos de los comandos    
*/


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

bool procesarEntrada(char *comando, tList *historical)
{
    bool terminado = false;

    updateHistorical(historical, comando);

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
    else if (strcmp(trozos[0], "historic") == 0)
        printHistorical(*historical);
    else if (strcmp(trozos[0], "exit") == 0 || strcmp(trozos[0], "quit") == 0 || strcmp(trozos[0], "bye") == 0)
        terminado = true;
    else if (strcmp(trozos[0], "help") == 0)
        helpCmd(trozos[1]);
    else if (strcmp(trozos[0], "infosys") == 0)
        infosys(trozos[1]);
    else
        printf("El comando %s no está definido.\n", trozos[0]);
    // else if (strcmp(trozos[0], "date") == 0)
    //     //dateCmd(trozos[1]);
    // else if (strcmp(trozos[0], "hour") == 0)
    //     hourCmd(trozos[1]);



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
        printf("sofía.oubiña.falcon@udc.es\n");
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
        perror("\ngetcwd() error");
    }
}

void changeDir(char *path) {
    if (path == NULL) {
        printCurrentDir();
        return;
    }
    int result = chdir(path);

    if (result != 0) {
        perror("Imposible cambiar de directorio");
    }

}

void updateHistorical(tList *historical, char *command) {
     tItemL item = (tItemL)malloc((strlen(command) + 1) * sizeof(char));
    strcpy(item, command);
    bool insertado = insertItem(item, LNULL, historical);
    printf("Comando %s insertado en el historial: %s\n", insertado ? "" : "no", command);
}

void printHistorical(tList historical) {
    tPosL pos = first(historical);
    int index = 1;
    while (pos != LNULL) {
        printf("%d: %s\n", index, getItem(pos, historical));
        pos = next(pos, historical);
        index++;
    }
}

void infosys(char *mod) {
    (void)mod;
    struct utsname sys_info;

    if (uname(&sys_info) == -1) {
        printf("Error: no se pudo obtener la información del sistema\n");
        return;
    }
    printf("Información del sistema:\n");
    printf("Sistema Operativo: %s\n", sys_info.sysname);
    printf("Nombre del Nodo: %s\n", sys_info.nodename);
    printf("Versión del Sistema: %s\n", sys_info.version);
    printf("Release del Sistema: %s\n", sys_info.release);
    printf("Arquitectura de la Máquina: %s\n", sys_info.machine);
}

void helpCmd(char *cmd) {
    if (cmd == NULL) {
        printf("Lista de comandos disponibles:\n");
        printf("authors [-l|-n]\n");
        printf("getpid [-p]\n");
        printf("chdir [dir]\n");
        printf("getcwd\n");
        printf("infosys\n");
        printf("help [cmd]\n");
        printf("exit | quit | bye\n");
    } else if (strcmp(cmd, "authors") == 0) {
        printf("authors [-l|-n]: Muestra los autores. -l logins, -n nombres.\n");
    } else if (strcmp(cmd, "getpid") == 0) {
        printf("getpid [-p]: Muestra el pid del shell o el de su padre.\n");
    } else if (strcmp(cmd, "chdir") == 0) {
        printf("chdir [dir]: Cambia el directorio actual. Sin argumento lo muestra.\n");
    } else if (strcmp(cmd, "getcwd") == 0) {
        printf("getcwd: Imprime el directorio actual.\n");
    } else if (strcmp(cmd, "infosys") == 0) {
        printf("infosys: Muestra información básica del sistema.\n");
    } else if (strcmp(cmd, "help") == 0) {
        printf("help [cmd]: Muestra todos los comandos o ayuda sobre un comando concreto.\n");
    } else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0 || strcmp(cmd, "bye") == 0) {
        printf("exit | quit | bye: Termina la ejecución del shell.\n");
    } else {
        printf("No hay ayuda disponible para '%s'\n", cmd);
    }
}






