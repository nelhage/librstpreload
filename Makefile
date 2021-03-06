CFLAGS := -O2 -g -fPIC -Wall

librstpreload.so: rstpreload.o rst.o
	$(LD) --version-script ver -shared $^ -ldl -lc -o $@

dsttest: dsttest.o rst.o

clean:
	-rm -f *.o librstpreload.so dsttest *~

install: librstpreload.so rst-run.sh mkrst.sh
	install -D -m a=rx,u=rwx -p librstpreload.so /usr/local/lib/librstpreload.so
	install -D -m a=rx,u=rwx -p rst-run.sh /usr/local/bin/rst-run
	install -D -m a=rx,u=rwx -p mkrst.sh /usr/local/bin/mkrst
