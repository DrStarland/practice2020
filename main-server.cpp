#include <iostream>
#include "socket.h"
#include <string.h>
#include "udpbasedtransmittion.h"

bool socket_test();

using std::cin; using std::cout; using std::endl;

int main()
{
    UDPBasedTransmittion system(27000);

    Address destination(127, 0, 0, 1, 26000);
    //Address destination(176, 195, 28, 243, 26000);

    char name[30];
    cout << "activity test " << system.isActive() << endl;
    сout << "Input name of file: \n";
    cin >> name;

    cout << "Result of sending: "<< system.sendFile(destination, name);

    cout << "\nall";
    return 0;
}