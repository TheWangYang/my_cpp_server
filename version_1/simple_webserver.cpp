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

    //定义socket套接字对象
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
        //接收客户端的连接请求，并创建套接字对象
        connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
        printf("here 1");

        //表示接收到错误的套接字请求
        if (connfd == INVALID_SOCKET) {
            fprintf(stderr, "Accept failed.\n");
        } else {
            //接收到正确的套接字请求
            char request[1024];

            //得到recv函数返回字节数结果
            int bytesReceived = recv(connfd, request, sizeof(request) - 1, 0);
            
            //如果字节数为-1表示接收错误
            if (bytesReceived == -1) {
                // 处理接收错误
                printf("接收错误");
            } else {
                //接收成功
                request[bytesReceived] = '\0'; // 手动添加 null 终止字符
                printf("%s\n", request);
                printf("Request Successfully!\n");

                //HTTP响应头
                char buf[520] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n"; // HTTP响应
                int s = send(connfd, buf, strlen(buf), 0); // 发送响应

                // 发送响应头之后，继续发送响应数据内容
                // 调用.html界面
                FILE *file = fopen("E:\\_my_github_repositories\\my_cpp_server\\version_1.0\\html\\hello.html", "rb");
                if (file) {
                    fseek(file, 0, SEEK_END);//将文件指针移动到最后
                    long fileSize = ftell(file);//得到文件指针对应的开始位置的偏移量（就是文件长度）
                    fseek(file, 0, SEEK_SET);//将文件指针重新放回到文件的开始位置
                    //给文件分配内存地址
                    char *fileBuffer = (char *)malloc(fileSize);

                    //如果分配地址成功
                    if (fileBuffer) {
                        //读取文件内容
                        fread(fileBuffer, 1, fileSize, file);
                        //发送到客户端界面进行HTML页面的显示，其中参数0表示默认行为
                        send(connfd, fileBuffer, fileSize, 0);
                        // 释放文件buffer缓存地址
                        free(fileBuffer);
                    }
                    // 关闭文件指针
                    fclose(file);
                }
            }

            // 关闭套接字对象
            closesocket(connfd);
        }
    }

    printf("here 3");
    //关闭服务器端套接字对象
    closesocket(sock);
    //清理windows套接字编程需要的资源，为了避免不必要的资源泄露情况出现
    WSACleanup();
    
    return 0;
}
