#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3000
#define MAX_CLIENTS 4

int main() {
    int serverSocket, clientSockets[MAX_CLIENTS];
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[1024];
    int numClients = 0, clientAddressLength, bytesReceived, i;

    // Kreiranje server soketa
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        printf("Greška: neuspješno kreiranje server soketa\n");
        return 1;
    }

    // Konfiguracija adrese servera
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Povezivanje server soketa s adresom
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("Greška: neuspješno povezivanje server soketa s adresom\n");
        close(serverSocket);
        return 1;
    }

    // Slušanje zahtjeva za povezivanje
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        printf("Greška: neuspješno postavljanje server soketa u mod slušanja\n");
        close(serverSocket);
        return 1;
    }

    // Prihvaćanje klijentskih zahtjeva za povezivanje
    while (numClients < MAX_CLIENTS) {
        printf("Čekanje zahtjeva za povezivanje klijenta...\n");
        clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddressLength);
        if (clientSocket == -1) {
            printf("Greška: neuspješno prihvaćanje klijentskog zahtjeva za povezivanje\n");
            close(serverSocket);
            return 1;
        }

        // Čitanje imena klijenta
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            printf("Greška: neuspješno čitanje imena klijenta\n");
            close(clientSocket);
            continue;
        }
        buffer[bytesReceived] = '\0';

        // Povećanje broja klijenata
        numClients++;

        // Spremanje soketa
        clientSockets[numClients - 1] = clientSocket;
        // Slanje poruke o broju klijenata
        sprintf(buffer, "B%d", numClients);
        send(clientSocket, buffer, strlen(buffer), 0);

        // Zatvaranje konekcije s klijentom
        printf("N%s\n", buffer + 1); // Ispis imena klijenta
    }

    // Slanje poruke "P" svim klijentima
    for (i = 0; i < MAX_CLIENTS; i++) {
        sprintf(buffer, "P");
        send(clientSockets[i], buffer, strlen(buffer), 0);
}

// Čekanje odgovora "OK" od svakog klijenta
for (i = 0; i < MAX_CLIENTS; i++) {
    bytesReceived = recv(clientSockets[i], buffer, sizeof(buffer), 0);
    if (bytesReceived == -1) {
        printf("Greška: neuspješno čitanje odgovora od klijenta %d\n", i + 1);
        close(clientSockets[i]);
        continue;
    }
    buffer[bytesReceived] = '\0';
    if (strcmp(buffer, "OK") != 0) {
        printf("Greška: klijent %d nije potvrdio primanje poruke\n", i + 1);
    } else {
        printf("OK%d\n", i + 1);
    }
    close(clientSockets[i]);
}

// Zatvaranje server soketa
close(serverSocket);

return 0;
}