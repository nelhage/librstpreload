CFLAGS := -O2 -g -fPIC -Wall
librstpreload.so: rstpreload.o
	$(LD) --version-script ver -shared $^ -ldl -lc -o $@
rstpreload.o: rstpreload.c
clean:
	-rm -f *.o librstpreload.so
