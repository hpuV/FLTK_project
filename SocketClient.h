#ifndef SOCKET
#define SOCKET

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

using std::string;

class SocketClient {
public:
    SocketClient(string _host, int _port) {
        host = _host;
        port = _port;
    }
	SocketClient() {}
	~SocketClient() {
		close(socket_fd);
	}
    int init ();
	void sendMessage(string);
	string getMessage(int);
	int getFd() const;
	void setSocket(string,int);
protected:
    string host;
    int port;
private:
    struct sockaddr_in serv_addr;
    int socket_fd;
    int valread;
};

int SocketClient::init () {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		// cout<<"Socket creation error"<<endl;
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <=0) {
		// cout<<"Invalid address Address not supported"<<endl;
		return -1;
	}
	if (connect(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0 ) {
		// cout<<"Connection Failed"<<endl;
		return -1;
	}
	return 0;
}

void SocketClient::sendMessage(string mesg) {
	const char* buffer = mesg.c_str();
	send(socket_fd, buffer, mesg.length(), 0);
}

string SocketClient::getMessage(int length=1024) {
	char buffer[length] = "";
	valread = read(socket_fd, buffer, length);
	return string(buffer);
}

int SocketClient::getFd() const {
	return socket_fd;
	}
void SocketClient::setSocket(string _host,int _port) {
	host = _host;
	port = _port;
}

#endif
