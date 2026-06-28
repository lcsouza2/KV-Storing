CC = gcc

CFLAGS = -Wall -Wextra -Werror -I./include
LDFLAGS =

SRC = src/sstables.c src/logging.c
OBJ = $(SRC:.c=.o)

all: kvstore

kvstore: $(OBJ) main.c
	$(CC) $(CFLAGS) -o kvstore main.c $(OBJ) $(LDFLAGS)

test: $(OBJ) tests/main.c
	$(CC) $(CFLAGS) -o run_tests tests/main.c $(OBJ) $(LDFLAGS)
	./run_tests

stress:
	@echo "Implementar script de stress test aqui depois"

clean:
	rm -f src/*.o kvstore run_tests
	rm -f *.dat L*/*.dat