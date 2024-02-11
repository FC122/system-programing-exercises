#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 3000 // Proizvoljni port
#define SERVER_ADDRESS "127.0.0.1" // Adresa servera

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[1024];
    ssize_t bytesReceived;

    // Kreiranje klijentskog soketa
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        printf("Greška: neuspješno kreiranje klijentskog soketa\n");
        return 1;
    }

    // Konfiguracija adrese servera
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_ADDRESS, &(serverAddress.sin_addr)) <= 0) {
        printf("Greška: neuspješna konverzija adrese servera\n");
        close(clientSocket);
        return 1;
    }

    // Spajanje na server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("Greška: neuspješno povezivanje na server\n");
        close(clientSocket);
        return 1;
    }

    // Slanje imena klijenta na server
    strcpy(buffer, "Nime");
    send(clientSocket, buffer, strlen(buffer) + 1, 0);

    // Čitanje poruke o broju klijenata
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == -1) {
        printf("Greška: neuspješno čitanje poruke o broju klijenata\n");
        close(clientSocket);
        return 1;
    }
    buffer[bytesReceived] = '\0';

    // Ispis poruke o broju klijenata
    printf("B%c\n", buffer[1]);

    // Čitanje poruka od servera
    while (1) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            printf("Greška: neuspješno čitanje poruke od servera\n");
            break;
        } else if (bytesReceived == 0) {
            printf("Server je zatvorio konekciju.\n");
            break;
        }
        buffer[bytesReceived] = '\0';
        // Provjera je li primljena poruka "P" od servera
        if (strcmp(buffer, "P") == 0) {
            printf("P\n");
            break;
        }
    }

    // Slanje "OK" poruke na server
    strcpy(buffer, "OK");
    send(clientSocket, buffer, strlen(buffer) + 1, 0);

    // Zatvaranje klijentskog soketa
    close(clientSocket);
    return 0;
}
