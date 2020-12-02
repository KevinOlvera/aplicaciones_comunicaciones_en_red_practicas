#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    struct sockaddr_in server, client;
    char buffer[50];
    
    if (argc != 2) {
        printf("ERROR: Insufficient arguments");
        return -1;
    }

    int sck = socket(AF_INET, SOCK_STREAM, 0);

    if (sck == -1) {
        perror("ERROR");
        return -1;
    }

    //2 Hacer publico el servicio
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sck, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("ERROR");
        return -1;
    }
    
    //3 el servicod empieza a escuchar
    if (listen(sck, 2) == -1) {
        perror("ERROR");
        return -1;
    }

    while (1) {
        //Aceptar las conexiones, fd = file descriptor for the socket accepted
        int client_size = sizeof(client);
        int fd = accept(sck, (struct sockaddr *)&client, &client_size);
        if (fd == -1) {
            perror("ERROR");
            close(sck);
            return -1;
        } 

        while (1) {
            //Fase de comunicacion
            int recv_bytes = recv(fd, buffer, sizeof(buffer), 0);
            if (recv_bytes == -1) {
                perror("ERROR");
                return -1;
            }

            buffer[recv_bytes] = '\0';

            if (strcmp(buffer, "Fin") == 0) {
                close(fd);
                break;
            }

            printf("Client: %s\n", buffer);

            printf("Server: ");
            //scanf("%50[^\n]", buffer);
            fgets(buffer, 50, stdin);
            buffer[strlen(buffer)-1] = '\0';
            //strcpy(buffer, "Hola Kevin Olvera - S");

            int sent_bytes = send(fd, buffer, strlen(buffer), 0);
            if (sent_bytes == -1) {
                perror("ERROR");
                return -1;
            } else if (sent_bytes != strlen(buffer)) {
                printf("ERROR: The message was sent incompleted");
                return -1;
            }

            //printf("El servidor envio: %s\n", buffer);
            if (strcmp(buffer, "Fin") == 0) {
                close(fd);
                break;
            }
        }
        
    }

    return 0;
}
