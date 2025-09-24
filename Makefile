CC = gcc
CFLAGS = -Wall
DEB = gdb
DEBFLAGS = -g -O0

SRC = ./src/p0.c
OBJ = p0.out
LIB = ./src/lista_comandos.c

COM = ./src/utils.c

# Se compila y se ejecuta el programa
all: $(OBJ)
	./$(OBJ)

# Se compila el programa
compile: $(OBJ)

# Se limpia el directorio de archivos compilados
clean:
	rm -f *.out

debug:
	$(CC) $(DEBFLAGS) $(SRC) $(COM) $(LIB)
	$(DEB) a.out

# Compilacion del programa
$(OBJ): $(SRC) $(COM)
	$(CC) $(CFLAGS) -o $(OBJ) $(SRC) $(COM) $(LIB)