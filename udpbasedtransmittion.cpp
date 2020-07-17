#include "udpbasedtransmittion.h"

bool UDPBasedTransmittion::isActive() { return state; }

UDPBasedTransmittion::UDPBasedTransmittion(port_number port) {
    m_socket.open(port);
    this->state = bool(m_socket);
}

UDPBasedTransmittion::~UDPBasedTransmittion() {
    m_socket.close();
}

bool UDPBasedTransmittion::send(const Address &destination, const void *data, int size) const {
    return m_socket.send(destination, data, size);
}
int UDPBasedTransmittion::receive(Address &sender, void *buffer, int size) const {
    return m_socket.receive(sender, buffer, size);
}

bool UDPBasedTransmittion::sendFile(const Address &destination, const char *filename) {
    if (!isActive())
        return false;
    cout << "?\n";
    FileDescriptor file(filename, "rb");

    if (!bool(file))
        return false; //throw
    cout << "??\n";
    FileDeclaration order(filename, "", file.countPackets(MAX_PACKET_LEN));
    int my_ID = order.getID();
    if (!send(destination, &order, sizeof(FileDeclaration)))
        return false;
    cout << "???\n";
    AnswerDeclaration answer(0, 0);
    Address sender;

    FileDescriptor tmp("testtest.png", "wb");
    for (size_t i = 0; i < 15000000; i++)
        if (receive(sender, &answer, sizeof(AnswerDeclaration)) > 0) {
            cout << "chto-to prislo\n";
            if (true || (sender == destination && answer.cmp_ID(my_ID) && bool(answer))) {
                //sending_process()
                cout << "????\n";
                for (size_t i = 0; i < order.getPacketAmount(); ++i) {
                    Infopacket buffer;
                    buffer.reset(file, i, my_ID);
                    send(destination, &buffer, sizeof(Infopacket));
                    tmp.setPacket(buffer.getData(), buffer.getDataLength(), i);
                }
                return true;
            }
            else
                cout << "answer hasn`t came..." << std::endl;
            break;
        }
    cout << "cho-to u clienta\n";
    return false;
}

bool UDPBasedTransmittion::receiveFile(FileDeclaration &order, const char *another_name) {
    Address sender;
    cout << "!\n";
    int temp;
    char order_pl[2000];
    for (size_t i = 0; i < 150000000; i++)
    {
        if ((temp = m_socket.receive(sender, &order, sizeof(FileDeclaration))) > 0) {
            if (temp == sizeof(FileDeclaration)) {
                cout << "!!!!!!!!!!!!!!!!!!!\n";
                AnswerDeclaration verdict(22222, true);
                cout << send(sender, &verdict, sizeof(AnswerDeclaration)) << std::endl;
                break;
            }
        }
    }

    FileDescriptor file(another_name == NULL ? order.getName() : another_name, "wb+");
    Infopacket buf_pack;
    std::vector<unsigned char> list_of_lack = {};
    list_of_lack.resize(order.getPacketAmount());
    for (unsigned char& i : list_of_lack)
        i = 0; // proverit`
    cout << "!!!\n";
    size_t control_sum = order.getPacketAmount();
    for (size_t j = 0, i = 0; i < 15000000 && control_sum; i++) {
        if (receive(sender, &buf_pack, sizeof(Infopacket))) {
            list_of_lack[buf_pack.getNumber()] = 1;
            UDPbuffer.push_back(buf_pack);
            --i, --control_sum;
            file.setPacket(buf_pack.getData(), buf_pack.getDataLength(), j++);
            cout << "/////\n";
        }
    }
    size_t j = 0;
    for (Infopacket& i : UDPbuffer) {
        file.setPacket(i.getData(), i.getDataLength(), j++);
    }
    j = 0;
    for (unsigned char& i : list_of_lack) {
        if (i == 0)
            cout << "Achtung " << j << " ";
        ++j;
    }
    cout << std::endl;
    cout << "!!!!\n";
//    for (size_t i = 0; i < order.getPacketAmount(); i++) {
//        if (!(m_socket.receive(sender, &buf_pack.getDatagram(), sizeof(Infopacket::packet_t)) < 0)) {
//            list_of_lack[buf_pack.getNumber()] = 1;
//            file.setPacket(buf_pack.getData(), buf_pack.getDataLength(), i);
//        }
//    }

    return true;
}

bool UDPBasedTransmittion::receiveDeclaration(FileDeclaration &order, Address &sender) {
    int temp;
    for (size_t i = 0; i < 15000000; i++)
    {
        if ((temp = m_socket.receive(sender, &order, sizeof(FileDeclaration))) > 0) {
            if (temp == sizeof(FileDeclaration)) {
                AnswerDeclaration verdict(order.getID(), true);
                m_socket.send(sender, &verdict, sizeof(AnswerDeclaration));
                return true;
            }
        }
    }
    return false;
}
