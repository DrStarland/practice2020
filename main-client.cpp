#include <iostream>
#include "socket.h"
#include <string.h>
#include "udpbasedtransmittion.h"

using std::cin; using std::cout; using std::endl;

//switch (command)
//{
//    case DRAW:
//        code = viewFigure(stuff, canv);
//        break;
//    case MOVE:
//        code = move_model(stuff, &(parameters.shift));
//        break;
//    default:
//        code = UNKNOWN_OPERATION;
//}

//int main(int argc, char *argv[])
int main()
{
    UDPBasedTransmittion system(26000);
    //system.sendFile(destination, "test.png");
    FileDeclaration order;
    cout << "activity test " << system.isActive() << endl;
    for (int i = 0; i < 150000000; i++)
        if (system.receiveFile(order, "test2.png"))
            break;

    cout << "\nall";
    return 0;
}

//for (int i = 0; i < 10; i++) {
//    Address sender;
//    char buffer[25];
//    if (system.receive( sender, buffer, 25 ) > 0) {
//        cout << buffer << endl;
//    }
//    else
//        break;
//}
