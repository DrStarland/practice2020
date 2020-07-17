#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include "address.h"
#include "socketcontroller.h"

class Socket
{
    typedef bool SocketState;
public:
    Socket() = default;
    explicit Socket(const Socket &copy) = delete;
    Socket(Socket &&copy) noexcept;
    ~Socket();
    Socket& operator=(const Socket& copy) = delete;
    Socket& operator=(Socket&& copy);
    bool open(unsigned short port = 0);
    void close();
    bool isOpen() const;
    bool send(const Address &dest, const void *data, int size) const;
    int receive(Address &sender, void *data, int size) const;
    explicit operator bool() const noexcept;
private:
    // мои сокеты всегда неблокирующие, но есть возможность поправить это
    bool setToNonBlockingMode();
    SOCKET handle;
    SocketState isReady;
    SocketState isBlockMode;
};

#endif // SOCKET_H
