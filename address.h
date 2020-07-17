#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>

class Address
{
    typedef unsigned char byte;
public:
    // конструктор
    Address() = default;
    Address(byte a, byte b, byte c, byte d, unsigned short port);
    Address(unsigned int address, unsigned short port);
    // копирующие конструкторы
    explicit Address(const Address &copy);
    Address(Address &&copy) noexcept;
    Address& operator=(const Address& copy);
    Address& operator=(Address&& copy);
    ~Address() = default;
    byte getA() const noexcept;
    byte getB() const noexcept;
    byte getC() const noexcept;
    byte getD() const noexcept;
    void setA(byte x) noexcept;
    void setB(byte x) noexcept;
    void setC(byte x) noexcept;
    void setD(byte x) noexcept;
    unsigned int getAddress() const noexcept;
    unsigned short getPort() const noexcept;
    void setAddress(unsigned int addr) noexcept;
    void setPort(unsigned short) noexcept;
    bool operator== (const Address &other) const;
    bool operator!= (const Address &other) const;
    // планирую сделать проверку на нормальность адреса или просто на инициализацию адреса
    explicit operator bool() const noexcept;
    friend std::ostream& operator<< (std::ostream &out, const Address &matr);
private:
    unsigned int makeAddress(byte a, byte b, byte c, byte d) const noexcept;
    unsigned int address;
    unsigned short port;
};

#endif // ADDRESS_H
