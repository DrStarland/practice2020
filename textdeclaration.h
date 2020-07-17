#ifndef TEXTDECLARATION_H
#define TEXTDECLARATION_H

#include "basepacket.h"
#include <memory>

class TextDeclaration : public BasePacket
{
public:
    TextDeclaration(size_t len);
    TextDeclaration(const TextDeclaration &copy);
    virtual ~TextDeclaration() = default;
};

#endif // TEXTDECLARATION_H
