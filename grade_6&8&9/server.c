#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_CLIENTS 10

typedef struct {
    struct sockaddr_in clientAddr;
    socklen_t addrLen;
} ClientInfo;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int serverSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];
    ClientInfo clients[MAX_CLIENTS];
    int numClients = 0;

    // Создание UDP сокета
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(argv[1]));

    // Привязка адреса к сокету
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", atoi(argv[1]));

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        // Принятие сообщения от клиента
        ssize_t bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (bytesRead == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // Вывод информации о клиенте
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        printf("Received message from client %s:%d\n", clientIP, ntohs(clientAddr.sin_port));
        printf("%s\n", buffer);

        // Поиск клиента в списке клиентов
        int foundClient = -1;
        for (int i = 0; i < numClients; i++) {
            if (memcmp(&clientAddr, &(clients[i].clientAddr), sizeof(clientAddr)) == 0) {
                foundClient = i;
                break;
            }
        }

        // Добавление нового клиента в список, если не найден
        if (foundClient == -1 && numClients < MAX_CLIENTS) {
            memcpy(&(clients[numClients].clientAddr), &clientAddr, sizeof(clientAddr));
            clients[numClients].addrLen = clientAddrLen;
            numClients++;
        }

        // Отправка ответа всем клиентам
        for (int i = 0; i < numClients; i++) {
            ssize_t bytesSent = sendto(serverSocket, buffer, bytesRead, 0, (struct sockaddr*)&(clients[i].clientAddr), clients[i].addrLen);
            if (bytesSent == -1) {
                perror("Send failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    close(serverSocket);

    return 0;
}
