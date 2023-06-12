#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(strtol(argv[2], NULL, 10));
    if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    sprintf(buffer, "hi i am client\n");
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    memset(buffer, 0, sizeof(buffer));


    srand(time(NULL));
    if (strcmp(argv[3], "gardener") == 0) {
        int client_number = strtol(argv[4], NULL, 10);
        while (1) {
            for (int i = 0; i < 40; ++i) {
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "%d %d ", client_number, i);
                usleep(10000);
                ssize_t bytesSent = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
                if (bytesSent <= 0)
                    break;
                memset(buffer, 0, sizeof(buffer));
                socklen_t serverAddrLen = sizeof(serverAddr);
                ssize_t bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*) &serverAddr, &serverAddrLen);
                if (bytesRead <= 0)
                    break;
                if (buffer[0] == '1' || buffer[0] == '2') {
                    continue;
                }
                printf("%s", buffer);
            }
        }
    } else if (strcmp(argv[3], "flowerbed") == 0) {
        int flowers[40];
        for (int i = 0; i < 40; ++i) {
            flowers[i] = (((i + 1) * rand()) % 255) + 1;
            if (flowers[i] < 0) {
                flowers[i] *= -1;
            }
        }
        printf("Generated flowerbed:\n");
        for (int i = 0; i < 40; ++i) {
            if (i % 10 == 0) {
                printf("\n");
            }
            printf("%d ", flowers[i]);
        }
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            socklen_t serverAddrLen = sizeof(serverAddr);
            ssize_t bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*) &serverAddr, &serverAddrLen);
            if (bytesRead <= 0)
                break;
            char *end_ptr;
            if (buffer[0] != '1' && buffer[0] != '2') {
                continue;
            }
            int client_number = strtol(buffer, &end_ptr, 10);
            int flower_number = strtol(end_ptr, NULL, 10);
            printf("client number %d asks for status of flower number %d\n", client_number, flower_number);
            memset(buffer, 0, sizeof(buffer));
            if (flowers[flower_number] == 0) {
                sprintf(buffer, "flower number %d was watered by client %d\n", flower_number, client_number);
                flowers[flower_number] = ((rand() * 2) % 255) + 1;
                if (flowers[flower_number] < 0) {
                    flowers[flower_number] *= -1;
                }
            } else {
                sprintf(buffer, "flower number %d does not need watering\n", flower_number);
                flowers[flower_number]--;
            }
            ssize_t bytesSent = sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
            if (bytesSent <= 0) {
                break;
            }
        }
    } else if (strcmp(argv[3], "observer") == 0) {
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            socklen_t serverAddrLen = sizeof(serverAddr);
            ssize_t bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*) &serverAddr, &serverAddrLen);
            if (bytesRead <= 0)
                break;
            printf("%s", buffer);
        }
    }

    close(clientSocket);

    return 0;
}
