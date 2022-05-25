CC=gcc
CFLAGS=-Wall -g -std=c99
OBJECTS=main.o free_mem.o helpers.o tree.o

build: main 

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o sd_fs $(OBJECTS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c

free_mem.o: free_mem.c
	$(CC) $(CFLAGS) -c free_mem.c

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c


check: build
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./sd_fs

clean:
	rm sd_fs $(OBJECTS)
