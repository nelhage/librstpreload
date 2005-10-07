CFLAGS := -O2 -g -fPIC -Wall
mathpreload.so: mathpreload.o
	$(LD) --version-script ver -shared mathpreload.o -ldl -lc -o mathpreload.so
mathpreload.o: mathpreload.c
clean:
	-rm -f *.o mathpreload.so
