CC = gcc
CFLAGS = -Wall -Wextra 

SRCS = main.c serveur.c
OBJS = $(SRCS:.c=.o)

NAME = V0

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(NAME)


