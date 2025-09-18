#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./utils.h"

#define MAX_LENGTH 100

int main()
{
    bool terminado = false;

    while (!terminado)
    {
        char *comando = (char*)malloc(MAX_LENGTH * sizeof(char));

        imprimirPrompt();
        leerEntrada(comando);
        terminado = procesarEntrada(comando);
        
        free(comando);
    }
}