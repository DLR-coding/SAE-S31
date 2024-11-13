# Variables
CC = gcc
CFLAGS = -Wall -Iinclude
OBJ = src/main.o src/execute_demande.o
TARGET = serveur_sequentiel

# Compilation par défaut
all: $(TARGET)

# Compilation de l'exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compilation des fichiers .c en .o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers compilés
clean:
	rm -f $(OBJ) $(TARGET)
