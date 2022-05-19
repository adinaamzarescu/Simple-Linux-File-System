all: build

build:
	gcc -g main.c tree.c -o sd_fs

clean:
	rm *.o sd_fs

check: build
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./sd_fs


run:
	./sd_fs
