#ifndef SOCKET_CLIENT
#define SOCKET_CLIENT

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#define PORT 8080

using std::string;

class SocketClient{
private:
    string hostname;
    int port;
    int valread;
    int socket_fd;
    struct sockaddr_in serv_addr;
    
public:
    SocketClient(string h, int p) : hostname(h), port(p) {} 
    // create and connect to server
    
    int init();
    // sends a message
    
    void sendMessage(string message);
    // sends a command (message with '\n' at the end)
    
    //void sendCommand(string message);
    // receive a message from mud server
    
    string receiveMessage();
    
    // close the socket
    ~SocketClient()
    {
    close(socket_fd);
    }
};

int SocketClient::init(){
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            //printf("\n Socket creation error \n");
            return -1;
        }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, hostname.c_str(), &serv_addr.sin_addr)<=0) 
    {
        //printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        //printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}
void SocketClient::sendMessage(string message){

    const char *buffer = message.c_str();
    send(socket_fd , buffer, message.length() , 0 );

}

string SocketClient::receiveMessage(){
    char buffer[1024] = {0};
    valread = read( socket_fd , buffer, 1024);
    return string(buffer);
}

#endif