#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    struct sockaddr_in client;
    char buffer[50];
    int flag = 0;
    
    if (argc != 3) {
        printf("ERROR: Insufficient arguments");
        return -1;
    }

    int sck = socket(AF_INET, SOCK_STREAM, 0);

    if (sck == -1) {
        perror("ERROR");
        return -1;
    }

    //2 Hacer publico el servicio
    client.sin_family = AF_INET;
    client.sin_port = htons(atoi(argv[2]));
    client.sin_addr.s_addr = inet_addr(argv[1]);

    //Conexion con el servidor
    if (connect(sck, (struct sockaddr *)&client, sizeof(client)) == -1) {
        perror("ERROR");
        close(sck);
        return -1;
    }

    printf("Connected to %s:%d\n", inet_ntoa(client.sin_addr), htons(client.sin_port));

    //Fase de comunicacion

    while (1)
    {
        //strcpy(buffer, "Hola Kevin Olvera - C");
        printf("Client: ");
        //scanf("%50[^\n]", buffer);
        fgets(buffer, 50, stdin);

        buffer[strlen(buffer)-1] = '\0';

        int sent_bytes = send(sck, buffer, strlen(buffer), 0);
        if (sent_bytes == -1) {
            perror("ERROR");
            return -1;
        } else if (sent_bytes != strlen(buffer)) {
            printf("ERROR: The message was sent incompleted");
            return -1;
        }

        if (strcmp(buffer, "Fin") == 0) {
            close(sck);
            break;
        }

        //printf("El cliente envio: %s\n", buffer);

        int recv_bytes = recv(sck, buffer, sizeof(buffer), 0);
        if (recv_bytes == -1) {
            perror("ERROR");
            return -1;
        }

        buffer[recv_bytes] = '\0';

        if (strcmp(buffer, "Fin") == 0) {
            close(sck);
            break;
        }

        printf("Server: %s\n", buffer);
    }

    return 0;
}
