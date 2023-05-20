#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10

int clientSockets[MAX_CLIENTS];
pthread_t threads[MAX_CLIENTS];
pthread_mutex_t mutex;

void* handleClient(void* arg) {
    int clientSocket = *(int*)arg;
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
            break;

        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clientSockets[i] != -1 && clientSockets[i] != clientSocket) {
                send(clientSockets[i], buffer, bytesRead, 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clientSockets[i] == clientSocket) {
            clientSockets[i] = -1;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    close(clientSocket);
    free(arg);
    pthread_exit(NULL);
}

int main(int arc, char* argv[]) {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        clientSockets[i] = -1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(strtol(argv[1], NULL, 10));

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, addrLen) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %ld...\n", strtol(argv[1], NULL, 10));

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New client connected.\n");

        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (clientSockets[i] == -1) {
                clientSockets[i] = clientSocket;
                break;
            }
        }

        if (i == MAX_CLIENTS) {
            printf("Maximum number of clients reached. Connection rejected.\n");
            close(clientSocket);
        } else {
            int* newClientSocket = malloc(sizeof(int));
            *newClientSocket = clientSocket;
            if (pthread_create(&threads[i], NULL, handleClient, (void*)newClientSocket) != 0) {
                perror("Thread creation failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    pthread_mutex_destroy(&mutex);
    close(serverSocket);

    return 0;
}
