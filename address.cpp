#include "address.h"

Address::Address(byte a, byte b, byte c, byte d, unsigned short port) :
       Address( makeAddress(a, b, c, d), port)  {}

Address::Address(unsigned int address, unsigned short port) : address(address), port(port) {}

unsigned int Address::makeAddress(byte a, byte b, byte c, byte d) const noexcept {
    return (unsigned int) ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
}

bool Address::operator==(const Address &other) const {
    return (this->address == other.address && this->port == other.port);
}

bool Address::operator!=(const Address &other) const { return !(*this == other); }

unsigned int Address::getAddress() const noexcept  { return address; }
void Address::setAddress(unsigned int addr) noexcept { address = addr; }

unsigned short Address::getPort() const noexcept { return port; }
void Address::setPort(unsigned short port) noexcept { this->port = port; }


Address::byte Address::getA() const noexcept {
    //11111111 ******** ******** ********
    //******** ******** ******** 11111111
    //11111111
    return (byte) (address >> 24);
}
Address::byte Address::getB() const noexcept {
    return (byte) ((address & 0xFF0000) >> 16);
}
Address::byte Address::getC() const noexcept {
    return (byte) ((address & 0xFF00) >> 8);
}
Address::byte Address::getD() const noexcept {
    return (byte) ((address & 0xFF));
}
