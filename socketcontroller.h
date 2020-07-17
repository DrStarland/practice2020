#ifndef SOCKETCONTROLLER_H
#define SOCKETCONTROLLER_H

#include <winsock2.h>

// для контроля подключения библиотеки
class SocketController
{
    typedef bool SocketSystemState;
private:
    SocketSystemState state;
    static SocketSystemState init_state;
public:
    SocketController();
    ~SocketController();
    SocketSystemState isActivated() const;
};

#endif // SOCKETCONTROLLER_H
