/*Authors:
Sofía Oubiña Falcón - sofía.oubiña.@udc.es
Antonio Seoane de Ois - antonio.seoane.deois@udc.gal*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./utils.h"

#define MAX_LENGTH 100

int main()
{
    bool terminado = false;
    tList historical;
    createEmptyList(&historical);

    while (!terminado)
    {
        char *comando = (char*)malloc(MAX_LENGTH * sizeof(char));
        imprimirPrompt();
        leerEntrada(comando);
        terminado = procesarEntrada(comando, &historical);
        
        free(comando);
    }

    cleanListFromMemory(&historical);
    return 0;
}