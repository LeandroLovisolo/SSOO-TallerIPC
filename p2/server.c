#include "mt.h"

int ejecutar(char *comando, char *output, int output_size) {
	int pipes[2], child;

	/* Crear un pipe. */
	if (pipe(pipes) < 0) {
		perror("creando el pipe");
		return -1;
	}

	if ((child = fork()) == -1) {
		perror("realizando el fork");
		return -1;
	} else if (child != 0) {
		/* Se cierra el socket de escritura porque no lo usamos en el padre. */
		close(pipes[1]);

		/* Leemos los datos que manda el hijo desde el pipe. */
		if (read(pipes[0], output, output_size) < 0) {
			perror("leyendo datos");
			return -1;
		}

		/* Cerramos el pipe de lectura. */
		close(pipes[0]);
	} else {
		/* El hijo le manda datos a su padre a través del pipe. */
		/* Se cierra el socket de lectura porque no lo usamos en el padre. */
		close(pipes[0]);
		
		/* Reemplazo stdout y stderr por el socket de escritura. */
		dup2(pipes[1], 1);
		dup2(pipes[1], 2);

		/* Ejecuto el comando recibido. */
		system(comando);

		/* Fuerzo la escritura de los buffers de stdout y stderr. */
		fflush(stdout);
		fflush(stderr);
		
		/* Cerramos el pipe de escritura. */
		close(pipes[1]);

		/* Cerramos el proceso hijo. */
		exit(0);
	}

	return 0;
}

int main() {
	/* Crear socket sobre el que se lee: dominio INET, protocolo TCP. */
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("abriendo socket");
		exit(1);
	}

	/* Evitar el error "Address already in use". */
	int yes = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	    perror("setsockopt");
	    exit(1);
	} 

	/* Crear nombre, usamos INADDR_ANY para indicar que cualquiera puede enviar aquí. */
	struct sockaddr_in name;
	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(PORT);
	if (bind(sock, (void*) &name, sizeof(name))) {
		perror("haciendo bind");
		exit(1);
	}

	/* Escuchar en el socket. */
    if (listen(sock, 1) == -1) {
        perror("escuchando");
        exit(1);
    }

	/* Aceptar una conexión entrante. */
	struct sockaddr_storage remote;
	socklen_t len = sizeof(remote);
	int conn = accept(sock, (struct sockaddr*) &remote, &len);

    if (conn == -1) {
        perror("aceptando la conexión entrante");
        close(sock);
        exit(1);
    }

	/* Recibimos mensajes hasta que alguno sea el que marca el final. */
	char buf[MAX_MSG_LENGTH];
	for(;;) {
		ssize_t n = recv(conn, buf, MAX_MSG_LENGTH, 0);

        if (n < 0) { 
        	perror("recibiendo");
        	close(conn);
        	close(sock);
        	exit(1);
        }

        buf[n] = 0;
		if (strncmp(buf, END_STRING, MAX_MSG_LENGTH) == 0) break;

		printf("Ejecutando: %s", buf);

		/* Ejecuto el comando recibido. */
		char output[1024];
		memset(output, 0, sizeof(output));
		if(ejecutar(buf, output, sizeof(output)) != 0) {
			close(conn);
			close(sock);
			exit(1);
		}

		/* Envio su salida al cliente. */
		send(conn, output, strlen(output), 0);
	}

	/* Cerrar socket de conexión entrante. */
	close(conn);

	/* Cerrar socket de recepción. */
	close(sock);

	return 0;
}