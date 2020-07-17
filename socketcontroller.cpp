#include "socketcontroller.h"

SocketController::SocketSystemState SocketController::init_state = false;

SocketController::SocketController() {
    if (SocketController::init_state == false) {
        WSADATA WsaData;
        if (WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR)
            state = SocketController::init_state = true;
        else if (WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR)
            state = SocketController::init_state = true;
        else
            state = false;
    } else {
        state = true;
    }
}

SocketController::~SocketController() {
    if (isActivated())
        WSACleanup(), SocketController::init_state = false;
}

SocketController::SocketSystemState SocketController::isActivated() const {
    return state;
}
