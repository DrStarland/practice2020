#include <iostream>
#include "socket.h"
#include <string.h>
#include "udpbasedtransmittion.h"

bool socket_test();

using std::cin; using std::cout; using std::endl;

int main()
{
//    if (!socket_test()) {
//        std::cout << "eh";
//    }

    UDPBasedTransmittion system(27000);

    Address destination(127, 0, 0, 1, 26000);
    char name[] = "test.png";
    cout << "activity test " << system.isActive() << endl;
    {
        FileDescriptor file(name, "rb");
        cout << "test length " << file.countPackets(MAX_PACKET_LEN) << std::endl;
    }


    system.sendFile(destination, name);

    cout << "\nall";
    return 0;
}

//system.send(destination, name, 9);

//for (int i = 0; i < 10; i++) {
//    Address sender;
//    char buffer[25];
//    if (system.receive( sender, buffer, 25 ) > 0) {
//        cout << buffer << endl;
//    }
//    else
//        break;
//}

bool socket_test() {
    SocketController test;

    unsigned short port = 30000;
    Socket socket;
    socket.open(port);

    unsigned char a = 127, b = 0, c = 0, d = 1;
    Address target(a, b, c, d, port);

    const char packet_data[] = "hello world!";
    int packet_size = 14;

    int result = socket.send(target, packet_data, packet_size);
    if ( !result ) {
        printf( "failed to send packet" );
        return false;
    }
    else
        cout << " all okey " << result << endl;

    for (int i = 0; i < 10; i++) {
        Address sender;
        char buffer[256];
        if (socket.receive( sender, buffer, 255 ) > 0) {
            char buf2[14];
            strncpy(buf2, buffer, 14);
            cout << buf2 << std::endl;
        }
        else
            break;
    }

    socket.close();

    return true;
}

