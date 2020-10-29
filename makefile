
CC = gcc

CFLAGS += -std=gnu11
CFLAGS += -Wall -Werror

all: search parser

search: search.c 
	${CC} ${CFLAGS} -o search search.c -g

parser: main.c 
	${CC} ${CFLAGS} -o parser main.c -g


