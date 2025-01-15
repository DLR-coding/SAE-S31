CC = gcc
CFLAGS = -Wall -Wextra 

SRCS = main.c serveur.c
OBJS = $(SRCS:.c=.o)

NAME = V2_a

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(NAME)


