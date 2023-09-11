#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

// 定义端口号为8888
const int port = 8888;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock.\n");
        return 1;
    }

    SOCKET sock;
    SOCKET connfd;
    struct sockaddr_in sever_address;

    ZeroMemory(&sever_address, sizeof(sever_address));
    // 构造server对应的tcp socket信息
    sever_address.sin_family = AF_INET;
    sever_address.sin_addr.s_addr = INADDR_ANY;
    sever_address.sin_port = htons(port);

    // 得到套接字
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 如果socket小于0，返回错误
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create socket.\n");
        WSACleanup();
        return 1;
    }

    // 将套接字与服务器address进行绑定
    if (bind(sock, (struct sockaddr*)&sever_address, sizeof(sever_address)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 服务器监听对应的套接字
    if (listen(sock, 1) == SOCKET_ERROR) {
        fprintf(stderr, "Listen failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 循环等待客户端发送请求
    while (1) {
        struct sockaddr_in client;
        int client_addrlength = sizeof(client);
        connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
        printf("here 1");

        if (connfd == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed.\n");
        } else {
            char request[1024];
            int bytesReceived = recv(connfd, request, sizeof(request) - 1, 0);

            if (bytesReceived == -1) {
                // 处理接收错误
                printf("接收错误");
            } else {
                request[bytesReceived] = '\0'; // 手动添加 null 终止字符
                printf("%s\n", request);
                printf("Request Successfully!\n");

                char buf[520] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n"; // HTTP响应
                int s = send(connfd, buf, strlen(buf), 0); // 发送响应

                // 调用.html界面
                FILE *file = fopen("E:\\_my_github_repositories\\my_cpp_server\\version_1.0\\html\\hello.html", "rb");
                if (file) {
                    fseek(file, 0, SEEK_END);
                    long fileSize = ftell(file);
                    fseek(file, 0, SEEK_SET);

                    char *fileBuffer = (char *)malloc(fileSize);
                    if (fileBuffer) {
                        fread(fileBuffer, 1, fileSize, file);
                        send(connfd, fileBuffer, fileSize, 0);
                        free(fileBuffer);
                    }

                    fclose(file);
                }
            }

            closesocket(connfd);
        }
    }

    printf("here 3");
    closesocket(sock);
    WSACleanup();

    return 0;
}
