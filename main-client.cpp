#include <iostream>
#include "socket.h"
#include <string.h>
#include "udpbasedtransmittion.h"

using std::cin; using std::cout; using std::endl;

int main()
{

    unsigned short port = 26000;
    cout << "Input port (1025 - 65 535): ";
    //cin >> port;
    UDPBasedTransmittion system(port);
    if (system.isActive())
        cout << "All ready for receiving data.\n";
    else {
        cout << "Something has gone wrong...\n";
        return 1;
    }
    int over = 15000;
    cout << "Waiting has started.\n";
    for (int i = 0; i < over; i++) {
        Address sender; FileDeclaration order;
        if (system.receiveDeclaration(order, sender)) {
            if (system.receiveFile(order, NULL))
                cout << "File was successfully received.\n";
            break;
        }
        else
            cout << "But nothing came...\n";
        if (i % 8 == 0)
            cout << "Maybe should I waiting forever? Everything will be finished in "<< over - i << " cycles.\n";
    }

    cout << "\nQuiting...";
    return 0;
}
