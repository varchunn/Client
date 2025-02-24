#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>  // memset

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define DEFAULT_BUFLEN 512

int main() {
    int ConnectSocket;
    struct sockaddr_in serverAddr;
    char response[DEFAULT_BUFLEN];
    
    // Створення сокета
    ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ConnectSocket == -1) {
        cerr << "Помилка створення сокета!\n";
        return 1;
    }
    
    // Заповнення структури адреси сервера
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        cerr << "Помилка конвертації IP-адреси!\n";
        close(ConnectSocket);
        return 2;
    }
    
    // Підключення до сервера
    if (connect(ConnectSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Помилка підключення до сервера!\n";
        close(ConnectSocket);
        return 3;
    }
    
    cout << "Підключено до сервера!\n";
    
    // Отримання даних від сервера
    ssize_t bytesReceived = recv(ConnectSocket, response, DEFAULT_BUFLEN - 1, 0);
    if (bytesReceived == -1) {
        cerr << "Помилка отримання даних!\n";
    } else if (bytesReceived == 0) {
        cout << "Сервер закрив з'єднання.\n";
    } else {
        response[bytesReceived] = '\0';  // виправлено
        cout << "Отримано від сервера: " << response << endl;
    }

    // Закриття сокета
    close(ConnectSocket);

    return 0;
}
