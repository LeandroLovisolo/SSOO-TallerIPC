.PHONY: all clean

# CFLAGS += -std=c99

OBJ = $(CC) $(CFLAGS) -c

BIN = $(CC) $(CFLAGS) -o $@

BIN_OBJS = $(CC) $(CFLAGS) $^ -o $@

all: bin/p1_server bin/p1_client bin/p2_server bin/p2_client \
	 bin/mt_server bin/cliente bin/display bin/display2 \
	 bin/display2_nonblocking bin/display2_select bin/pipe 

bin/p1_server: p1/mt.h p1/server.c
	$(BIN) p1/server.c

bin/p1_client: p1/mt.h p1/client.c
	$(BIN) p1/client.c

bin/p2_server: p2/mt.h p2/server.c
	$(BIN) p2/server.c

bin/p2_client: p2/mt.h p2/client.c
	$(BIN) p2/client.c

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
	rm -f bin/*