#ifndef ANSWERDECLARATION_H
#define ANSWERDECLARATION_H

#include "basepacket.h"

class AnswerDeclaration : BasePacket
{
private:
    bool verdict;
    int input_order_ID;
public:
    AnswerDeclaration(int input_order_ID, bool answer);
    virtual ~AnswerDeclaration() = default;
    explicit operator bool() const noexcept;
    bool cmp_ID(int ID);
};

#endif // ANSWERDECLARATION_H
