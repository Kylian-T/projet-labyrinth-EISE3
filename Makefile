# Options de compilation
CC = gcc
CFLAGS = -Wall


# Fichiers du projet
SRC = main.c labyrinthAPI.c clientAPI.c fonction_labyrinth.c
OBJ = $(SRC:.c=.o)
EXEC = labyrinth.out


# Règle initiale
all : $(EXEC)


# Dépendance des .h
main.o : main.c
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

labyrinthAPI.o : labyrinthAPI.c labyrinthAPI.h
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clientAPI.o : clientAPI.c clientAPI.h
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

fonction_labyrinth.o : fonction_labyrinth.c fonction_labyrinth.h
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<


# Règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) $(CCFLAGS) -g -o $@ $^


# Règles supplémentaires
clean:
	rm -f *.o
mproper:
	rm -f $(EXEC) *.o 
