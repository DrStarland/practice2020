#ifndef BASEPACKET_H
#define BASEPACKET_H

class BasePacket
{
protected:
    int ID;
public:
    BasePacket() = default;
    virtual ~BasePacket() = default;
    virtual int getID() const noexcept { return ID; }
};

#endif // BASEPACKET_H
