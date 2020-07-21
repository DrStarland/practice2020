#include "infopacket.h"

Infopacket::Infopacket(const void *data, size_t len, size_t number, int ID) {
    this->reset(data, len, number, ID);
}

bool Infopacket::reset(const void *data, size_t len, size_t number, int ID) {
    if (MAX_PACKET_LEN < len)
        return false;

    memcpy(this->data, data, len);
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

bool Infopacket::operator>(const Infopacket &inf) {
    return (this->packet_number > inf.packet_number);
}
bool Infopacket::operator<(const Infopacket &inf) {
    return (this->packet_number < inf.packet_number);
}
bool Infopacket::operator<=(const Infopacket &inf) {
    return (this->packet_number <= inf.packet_number);
}
bool Infopacket::operator>=(const Infopacket &inf) {
    return (this->packet_number >= inf.packet_number);
}

