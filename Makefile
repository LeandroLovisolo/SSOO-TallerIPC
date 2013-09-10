.PHONY: all clean

OBJ = $(CC) $(CFLAGS) -c

BIN = $(CC) $(CFLAGS) -o $@

BIN_OBJS = $(CC) $(CFLAGS) $^ -o $@

all: bin/mt_server bin/cliente bin/display bin/display2 \
	 bin/display2_nonblocking bin/display2_select bin/pipe 

bin/mt_server: mini-telnet/mt.h mini-telnet/mt_server.c
	$(BIN) mini-telnet/mt_server.c

bin/cliente: non-blocking/header.h non-blocking/cliente.c
	$(BIN) non-blocking/cliente.c

bin/display: non-blocking/header.h non-blocking/display.c
	$(BIN) non-blocking/display.c

bin/display2: non-blocking/header.h non-blocking/display2.c
	$(BIN) non-blocking/display2.c

bin/display2_nonblocking: non-blocking/header.h non-blocking/display2_nonblocking.c
	$(BIN) non-blocking/display2_nonblocking.c

bin/display2_select: non-blocking/header.h non-blocking/display2_select.c
	$(BIN) non-blocking/display2_select.c

bin/pipe: pipe/pipe.c
	$(BIN) pipe/pipe.c

clean:
	rm bin/*