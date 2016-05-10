#include "tcp.hpp"
#include "../exceptions/types.hpp"

#include <sys/socket.h>
#include <cstring>
#include <unistd.h>

Connection::TCP::TCP(TCPAddress const& address) {
    // Przygotowanie socketu
    struct sockaddr_in sock_addr = this->prepare_sockaddr(address);
    
    if ((this->_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        throw Exception::Error("Connection::TCP socket(): cannot create socket");
    }
    
    if (connect(this->_socket, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
        throw Exception::Error("Connection::TCP connect(): there's no connection between host and server");
    }
}

Connection::TCP::~TCP() {
    close(this->_socket);
}

struct sockaddr_in Connection::TCP::prepare_sockaddr(TCPAddress const& address) {
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof sock_addr);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr = address._ip_address;
    sock_addr.sin_port = address._port;
    
    return sock_addr;
}

void Connection::TCP::send(char *data) {
    if (write(this->_socket, data, strlen(data) + 1) < 0) {
        throw Exception::Warning(strerror(errno));
    }
}
