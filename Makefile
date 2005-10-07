CFLAGS := -O2 -g -fPIC -Wall
rstpreload.so: rstpreload.o
	$(LD) --version-script ver -shared rstpreload.o -ldl -lc -o rstpreload.so
rstpreload.o: rstpreload.c
clean:
	-rm -f *.o rstpreload.so
