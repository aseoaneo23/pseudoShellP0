#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#include "utils.h"
#include "lista.h"

#define MAX_HOST_LENGTH 256
#define TOPE 500

// static ModMap table[] = {
//     {"N", N},
//     {"-N", LAST_N},
//     {"-count", COUNT},
//     {"-clear", CLEAR},
//     {NULL, NO_MOD} // Marcar el final de la tabla
// };



void imprimirPrompt()
{
    char *userName = getlogin();
    char host[MAX_HOST_LENGTH];

    if (gethostname(host, sizeof(host)) != 0)
    {
        strcpy(host, "maquina_desconocida");
    }
    else if (userName == NULL)
    {
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
    else if (strcmp(trozos[0], "getpid") == 0)
        getShellPid(trozos[1]);
    else if (strcmp(trozos[0], "chdir") == 0)
        changeDir(trozos[1]);
    else if (strcmp(trozos[0], "getcwd") == 0)
        printCurrentDir();
    else if (strcmp(trozos[0], "historic") == 0)
        if (trozos[1] == NULL)
            printHistorical(*historical);
        else
            manageHistoricalWMods(trozos[1], historical);
    else if (strcmp(trozos[0], "exit") == 0 || strcmp(trozos[0], "quit") == 0 || strcmp(trozos[0], "bye") == 0)
        terminado = true;
    else if (strcmp(trozos[0], "help") == 0)
        helpCmd(trozos[1]);
    else if (strcmp(trozos[0], "infosys") == 0)
        infosys(trozos[1]);
    else if (strcmp(trozos[0], "date") == 0)
        dateCmd(trozos[1]);
    else if (strcmp(trozos[0], "close") == 0)
        closeCmd(trozos[1]);
    else if (strcmp(trozos[0], "dup") == 0)
        dupCmd(trozos[1]);
    else
        printf("Comando '%s' desconocido\n", trozos[0]);

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

void getShellPid(char *mod)
{
    if (mod != NULL && strcmp(mod, "-p") == 0)
    {
        printf("Pid del padre del shell: %d\n", getppid());
        return;
    }
    printf("Pid de shell: %d\n", getpid());
}

void printCurrentDir()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    else
    {
        perror("\ngetcwd() error");
    }
}

void changeDir(char *path)
{
    if (path == NULL)
    {
        printCurrentDir();
        return;
    }
    int result = chdir(path);

    if (result != 0)
    {
        perror("Imposible cambiar de directorio");
    }
}

bool updateHistorical(tList *historical, char *command)
{
    if (strcmp(command, "\n") == 0)
        return false;
        
    tItemL item = (tItemL)malloc((strlen(command) + 1) * sizeof(char));
    strcpy(item, command);
    bool insertado = insertItem(item, LNULL, historical);
    free(item);
    return insertado;
}

void printHistorical(tList historical)
{
    tPosL pos = first(historical);
    int i = 1;
    while (pos != LNULL)
    {
        printf("%d: %s\n", i, getItem(pos, historical));
        pos = next(pos, historical);
        i++;
    }
}

void customHistoricalPrint(MOD type, char *mod, tList historical) {

    tPosL pos = historical;
    int i = 1;
    int cmdAt = atoi(mod);
    printf("Mostrando comando %d del historial:\n", cmdAt);
    if (type == N) {
        while (i < cmdAt && pos != LNULL) {
            pos = next(pos, historical);
            i++;
        }

        tItemL command = getItem(pos, historical);
        printf("Ejecutando comando %s: ", command);
        
        if (command != NULL) {
            procesarEntrada(command, &historical);
        } else {
            printf("No hay comando en la posición %d del historial.\n", cmdAt);
        }
    } else if (type == LAST_N) {
        int lastN = atoi(mod + 1); // Saltar el '-' inicial
        tPosL lastPos = last(historical);
        while (lastN > 1 && lastPos != LNULL) {
            printf("%d: %s\n", lastN, getItem(lastPos, historical));
            lastPos = previous(lastPos, historical);
            lastN--;
        };
        
    }
}

void infosys(char *mod)
{
    (void)mod;
    struct utsname info;

    if (uname(&info) == -1)
    {
        printf("Error: no se pudo obtener la información del sistema\n");
        return;
    }
    printf("Información del sistema:\n");
    printf("Sistema Operativo: %s\n", info.sysname);
    printf("Nombre del Nodo: %s\n", info.nodename);
    printf("Versión del Sistema: %s\n", info.version);
    printf("Release del Sistema: %s\n", info.release);
    printf("Arquitectura de la Máquina: %s\n", info.machine);
}

void helpCmd(char *mod)
{
    if (mod == NULL)
    {
        printf("Lista de comandos disponibles:\n");
        printf("authors [-l|-n]\n");
        printf("getpid [-p]\n");
        printf("chdir [dir]\n");
        printf("getcwd\n");
        printf("date [-d|-t]\n");
        printf("hour\n");
        printf("historic [N|-N|-clear|-count]\n");
        printf("open [file] [mode]\n");
        printf("close [df]\n");
        printf("dup [df]\n");
        printf("listopen\n");
        printf("infosys\n");
        printf("help [cmd]\n");
        printf("exit | quit | bye\n");
    }
    else if (strcmp(mod, "authors") == 0)
    {
        printf("authors [-l|-n]: Muestra los autores. -l logins, -n nombres.\n");
    }
    else if (strcmp(mod, "getpid") == 0)
    {
        printf("getpid [-p]: Muestra el pid del shell o el de su padre.\n");
    }
    else if (strcmp(mod, "chdir") == 0)
    {
        printf("chdir [dir]: Cambia el directorio actual. Sin argumento lo muestra.\n");
    }
    else if (strcmp(mod, "getcwd") == 0)
    {
        printf("getcwd: Imprime el directorio actual.\n");
    }
    else if (strcmp(mod, "date") == 0)
    {
        printf("date [-d|-t]: Muestra la fecha y la hora actuales. -d solo fecha, -t solo hora.\n");
    }
    else if (strcmp(mod, "hour") == 0)
    {
        printf("hour: Muestra solo la hora actual (igual que date -t).\n");
    }
    else if (strcmp(mod, "historic") == 0)
    {
        printf("historic [N|-N|-clear|-count]: Gestiona o muestra el histórico de comandos.\n");
    }
    else if (strcmp(mod, "open") == 0)
    {
        printf("open [file] [modo]: Abre un fichero (cr, ap, ex, ro, rw, wo, tr). Sin argumentos lista abiertos.\n");
    }
    else if (strcmp(mod, "close") == 0)
    {
        printf("close [df]: Cierra el descriptor y lo quita de la lista de ficheros abiertos.\n");
    }
    else if (strcmp(mod, "dup") == 0)
    {
        printf("dup [df]: Duplica el descriptor de fichero y lo añade a la lista.\n");
    }
    else if (strcmp(mod, "listopen") == 0)
    {
        printf("listopen: Lista los ficheros abiertos (igual que open sin argumentos).\n");
    }
    else if (strcmp(mod, "infosys") == 0)
    {
        printf("infosys: Muestra información básica del sistema.\n");
    }
    else if (strcmp(mod, "help") == 0)
    {
        printf("help [cmd]: Muestra todos los comandos o la ayuda de un comando concreto.\n");
    }
    else if (strcmp(mod, "exit") == 0 || strcmp(mod, "quit") == 0 || strcmp(mod, "bye") == 0)
    {
        printf("exit | quit | bye: Termina la ejecución del shell.\n");
    }
    else
    {
        printf("Comando '%s' desconocido en help\n", mod);
    }
}

#define TAM_FECHA 20
#define TAM_HORA 20

void dateCmd(char *mod)
{
    time_t ahora = time(NULL);
    struct tm *lt = localtime(&ahora);
    char fecha[TAM_FECHA];
    char hora[TAM_HORA];

    if (ahora == (time_t)-1)
    {
        perror("time");
        return;
    }

    if (lt == NULL)
    {
        perror("localtime");
        return;
    }

    if (strftime(fecha, sizeof(fecha), "%d/%m/%Y", lt) == 0)
        return;
    if (strftime(hora, sizeof(hora), "%H:%M:%S", lt) == 0)
        return;

    if (mod == NULL)
    {
        printf("%s %s\n", fecha, hora);
    }
    else if (strcmp(mod, "-d") == 0)
    {
        printf("%s\n", fecha);
    }
    else if (strcmp(mod, "-t") == 0)
    {
        printf("%s\n", hora);
    }
    else
    {
        printf("Uso: date [-d|-t]\n");
    }
}

#define MAX_FICHEROS 100
#define MAX_NOMBRE 128

typedef struct
{
    int df;
    char name[MAX_NOMBRE];
    int modo;
    int ocupado;
} Fichero;

Fichero listaFicheros[MAX_FICHEROS];

void ListaFichAbiertos(void)
{
    int aux = 0;   // 0 = ninguno encontrado todavía
    for (int i = 0; i < MAX_FICHEROS; i++)
    {
        if (listaFicheros[i].ocupado)
        {
            printf("fd=%d  nombre=%s  flags=%d\n",
                   listaFicheros[i].df,
                   listaFicheros[i].name,
                   listaFicheros[i].modo);
            aux = 1;  // encontramos al menos uno
        }
    }
    if (aux == 0) {  // si no se encontró ninguno
        printf("Tabla de ficheros vacía\n");
    }
}


void EliminarFichAbiertos(int df)
{
    for (int i = 0; i < MAX_FICHEROS; i++)
    {
        if (listaFicheros[i].ocupado && listaFicheros[i].df == df) {
        listaFicheros[i].ocupado = 0;
        return;
        }
    }
}

char *NameFicheroDescriptor(int df)
{
    for (int i = 0; i < MAX_FICHEROS; i++)
    {
        if (listaFicheros[i].ocupado && listaFicheros[i].df == df)
        {
            return listaFicheros[i].name;
        }
    }
    return NULL;
}

void AnadirFicherosAbiertos(int df, const char *name, int modo)
{
    for (int i = 0; i < MAX_FICHEROS; i++)
    {
        if (!listaFicheros[i].ocupado)
        {
            listaFicheros[i].ocupado = 1;
            listaFicheros[i].df = df;
            listaFicheros[i].modo = modo;
            strncpy(listaFicheros[i].name, name, sizeof(listaFicheros[i].name) - 1);
            listaFicheros[i].name[sizeof(listaFicheros[i].name) - 1] = '\0';
            return;
        }
    }
    printf("La lista está llena, no se pudo registrar el fichero\n");
}

void closeCmd(char *mod)
{
    int df;

    if (mod == NULL || (df = atoi(mod)) < 0)
    {
        ListaFichAbiertos();
        return;
    }

    if (close(df) == -1)
    {
        perror("Imposible cerrar descriptor");
    }
    else
    {
        EliminarFichAbiertos(df);
    }
}

#define MAX_NAME 256

void dupCmd(char *mod)
{
    int df, duplicado;
    char aux[MAX_NAME];
    char *p;

    if (mod == NULL || (df = atoi(mod)) < 0)
    {
        ListaFichAbiertos();
        return;
    }
    p = NameFicheroDescriptor(df);

    if (p == NULL)
        p = "sin nombre registrado";
    duplicado = dup(df);

    if (duplicado == -1)
    {
        perror("dup");
        return;
    }

    sprintf(aux, "dup %d (%s)", df, p);
    AnadirFicherosAbiertos(duplicado, aux, fcntl(duplicado, F_GETFL));
    printf("%d -> %d\n", df, duplicado);
}

MOD identifyModifier(char *mod)
{
    int indexOfHistoric = atoi(mod);
    if (isdigit(mod[0]) && indexOfHistoric > 0)
        return N;
    else if (mod[0] == '-' && isdigit(mod[1]) && indexOfHistoric > 0)
        return LAST_N;
    else if (strcmp(mod, "-count") == 0)
        return COUNT;
    else if (strcmp(mod, "-clear") == 0)
        return CLEAR;
    else
        return NO_MOD;
}

void manageHistoricalWMods(char *mod, tList *historical) {
    MOD selectedMod = identifyModifier(mod);

    switch (selectedMod) {
        case N:
        case LAST_N:
            customHistoricalPrint(selectedMod, mod, *historical);
            break;
        case COUNT:
            // Implementar lógica para -count
            break;
        case CLEAR:
            cleanListFromMemory(historical);
            createEmptyList(historical);
            printf("Historial limpiado\n");
            break;
        default:
            printf("Modificador no manejado\n");
            break;
    }
}