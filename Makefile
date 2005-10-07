CFLAGS := -O2 -g -fPIC -Wall
librstpreload.so: rstpreload.o
	$(LD) --version-script ver -shared $^ -ldl -lc -o $@
rstpreload.o: rstpreload.c
clean:
	-rm -f *.o librstpreload.so

install: librstpreload.so
	install -D -m a=rx,u=rwx -p $< /usr/local/lib/librstpreload.so
