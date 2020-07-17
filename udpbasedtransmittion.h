#ifndef UDPBASEDTRANSMITTION_H
#define UDPBASEDTRANSMITTION_H

#include <iostream>
#include <memory>
#include "filedeclaration.h"
#include "answerdeclaration.h"
#include "socket.h"
#include "filedescriptor.h"
#include "address.h"
#include "infopacket.h"
#include <cmath>

using std::shared_ptr; using std::cin; using std::cout;

class UDPBasedTransmittion
{
    typedef unsigned short port_number;
public:
    UDPBasedTransmittion(port_number port = 30000);
    ~UDPBasedTransmittion();
    //bool sendMessage(char *str);
    bool send(const Address &destination, const void *data, int size) const;
    int receive(Address &sender, void *buffer, int size) const;
    bool sendFile(const Address &destination, const char *filename);
    bool receiveDeclaration(FileDeclaration &order, Address &sender);
    bool receiveFile(FileDeclaration &order, const char *another_name = NULL);
    bool isActive();
private:
    bool answerToTransmittion(bool command);
    SocketController _trigger;
    Socket m_socket;
    bool state;
    std::list<Infopacket> UDPbuffer;
};

#endif // UDPBASEDTRANSMITTION_H
