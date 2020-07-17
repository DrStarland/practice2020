#include "socket.h"

Socket::~Socket() {
    if (bool(*this)) {
        this->close();
    }
}

Socket::operator bool() const noexcept {
    return isReady;
}

bool Socket::setToNonBlockingMode() {
    DWORD nonBlocking = 1;
    if (ioctlsocket(this->handle, FIONBIO, &nonBlocking) != 0)
        return false;
    isBlockMode = false;
    return true;
}

bool Socket::open(unsigned short port) {
    if (port && port < 1025)
        return false;

    if ((handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
        return false;

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((unsigned short) port);

    if (bind(handle, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0)
        return false;

    if (!this->setToNonBlockingMode()) {
        closesocket(handle);
        return false;
    }

    isReady = true;
    return true;
}

void Socket::close() {
    closesocket(handle), isReady = false;
}

bool Socket::send(const Address &dest, const void *data, int size) const {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(dest.getAddress());
    address.sin_port = htons(dest.getPort());

    int sent_bytes = sendto(this->handle, (const char*) data, size,
                             0, (sockaddr*)&address, sizeof(sockaddr_in));
    if (sent_bytes != size)
        return false;
    return true;
}

int Socket::receive(Address &sender, void *data, int size) const {
    sockaddr_in from; int fromLength = sizeof(from);
    int rcvd_bytes = recvfrom(this->handle, (char*) data, size,
                               0, (sockaddr*)&from, &fromLength);
    if (rcvd_bytes <= 0)
        return 0;
    sender.setAddress(ntohl( from.sin_addr.s_addr));
    sender.setPort(ntohs(from.sin_port));
    return rcvd_bytes;
}
