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

    int s = socket(AF_INET, SOCK_STREAM, 0);

    connect(s, (const struct sockaddr *) &sin, sizeof(sin));

    for(;;) {
        printf("> ");

        /* Leo el comando ingresado por el usuario. */
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);

        /* Envío el comando al servidor. */
        send(s, buf, strlen(buf) + 1, 0);

        /* Leo el output del comando y lo imprimo en pantalla. */
        ssize_t n = recv(s, buf, sizeof(buf), 0);
        if (n < 0) { 
            perror("recibiendo");
            exit(1);
        }
        buf[n] = 0;
        printf("%s", buf);
    }

    close(s);

}