#include "mt.h"

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("Uso: %s <dirección IP>\n", argv[0]);
		return -1;
	}

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	if(inet_aton(argv[1], &sin.sin_addr) == 0) {
		printf("IP inválido.\n");
		return -1;
	}

	int s = socket(AF_INET, SOCK_DGRAM, 0);

	for(;;) {
		char buf[1024];
		printf("> ");
		fgets(buf, sizeof(buf), stdin);
		sendto(s, buf, strlen(buf) + 1, 0,
			(const struct sockaddr *) &sin, sizeof(sin));
	}

	close(s);

}