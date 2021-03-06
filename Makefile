main: LiDE.o nwio.o 
	gcc -o lide LiDE.o nwio.o -Wall -lcrypto -lssl

LiDE.o: LiDE.c 
	gcc -c LiDE.c

nwio.o: nwio.c 
	gcc -c nwio.c

clean: 
	rm LiDE.o nwio.o

clean-all: 
	rm lide LiDE.o nwio.o *~

install: lide
	cp lide /bin/lide

uninstall:
	rm -f /bin/lide
