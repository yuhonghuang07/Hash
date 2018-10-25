CC = gcc
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes
GDBFLAGS = -tui
EXEC = pruebas
FILES = hash.c testing.c main.c pruebas_catedra.c
ALL_FILES = *.c

build: $(FILES)
	$(CC) $(CFLAGS) $(ALL_FILES) -o $(EXEC)

run: build
	./pruebas

valgrind: ./$(EXEC)
	valgrind $(VFLAGS) ./$(EXEC)

gdb: all
	gdb $(GDBFLAGS) ./$(EXEC)

runtime: all
	time ./$(EXEC)

clean:
	rm -f *.o $(EXEC)

zip:
	zip $(EXEC).zip $(CFILES) $(HFILES)

script:
	chmod +x tiempos_volumen.sh
	./tiempos_volumen.sh ./pruebas

