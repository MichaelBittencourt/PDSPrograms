CC = gcc
FLAGS = -lm

RM=rm

SRC = $(wildcard *.c)
BINS = $(subst .c,.out,$(SRC))

all: $(BINS)

%.out: %.c
	$(CC) $< -o $@ $(FLAGS)


.PHONY: clean

clean:
	$(RM) *.out

