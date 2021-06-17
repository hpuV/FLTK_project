#include "Socket_Client.h"

int main(){

    SocketClient sc("127.0.0.1", 4000);
    sc.init();

    return 0;
}