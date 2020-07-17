#include "infopacket.h"

Infopacket::Infopacket(const void *data, size_t len, size_t number, int ID) {
    this->reset(data, len, number, ID);
}

//Infopacket::Infopacket(Infopacket &datagram) { *this = std::move(datagram); }

bool Infopacket::reset(const void *data, size_t len, size_t number, int ID) {
    if (MAX_PACKET_LEN < len)
        return false;

    for (size_t i = 0; i < len; i++)
        this->data[i] = ((char*) data)[i];
    packet_number = number;
    end = len;
    this->ID = ID;
    return true;
}

bool Infopacket::reset(FileDescriptor &file, size_t number, int ID) {
    end = (short) file.getPacket(data, number);
    packet_number = number;
    this->ID = ID;
    return true;
}

