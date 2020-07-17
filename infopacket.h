#ifndef INFOPACKET_H
#define INFOPACKET_H

#include <memory>
#include "filedescriptor.h"
#include "basepacket.h"
#include "constants.h"

class Infopacket : BasePacket
{
private:
    char data[MAX_PACKET_LEN];
    size_t packet_number;
    short end;
public:
    Infopacket() = default;
    Infopacket(const void *data, size_t len, size_t number, int ID);
    //Infopacket(Infopacket &datagram);
    virtual ~Infopacket() = default;
    bool reset(FileDescriptor &file, size_t number, int ID);
    bool reset(const void *data, size_t len, size_t number, int ID);
    char* getData() { return this->data; }
    short getDataLength() const { return this->end; }
    size_t getNumber() const { return this->packet_number; }

};

#endif // INFOPACKET_H
