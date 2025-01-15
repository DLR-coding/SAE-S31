CC = gcc
CFLAGS = -Wall -pthread
EXEC = V1_b

all: $(EXEC)

$(EXEC): main.o serveur.o
	$(CC) $(CFLAGS) -o $(EXEC) main.o serveur.o

main.o: main.c serveur.h
	$(CC) $(CFLAGS) -c main.c

serveur.o: serveur.c serveur.h
	$(CC) $(CFLAGS) -c serveur.c

clean:
	rm -f *.o $(EXEC)