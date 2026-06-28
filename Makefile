CC = gcc

CFLAGS = -Wall -Wextra -Werror -I./include
LDFLAGS =

SRC = src/sstables.c src/logging.c
OBJ = $(SRC:.c=.o)

TEST_SRC = tests/unit/*.c tests/main.c

all: kvstore

kvstore: $(OBJ) main.c
	$(CC) $(CFLAGS) -o kvstore main.c $(OBJ) $(LDFLAGS)

test: $(OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) -o run_tests $(TEST_SRC) $(OBJ) $(LDFLAGS)
	./run_tests

stress:
	@echo "Implementar script de stress test aqui depois"

clean:
	rm -f src/*.o kvstore run_tests
	rm -f *.dat L*/*.dat