# Simple Makefile for Project3

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = bst.c hashmap.c linked\_list.c main.c priority\_queue.c queue.c stack.c
OBJ = \$(SRC:.c=.o)
TARGET = quiz

all: \$(TARGET)

\$(TARGET): \$(SRC)
\$(CC) \$(CFLAGS) -o \$(TARGET) \$(SRC)

clean:
rm -f \$(TARGET) \$(OBJ)
