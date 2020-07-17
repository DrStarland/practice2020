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

    FileDescriptor file(filename, "rb");
    if (!bool(file))
        return false;

    FileDeclaration order(filename, "", file.countPackets(MAX_PACKET_LEN));
    int my_ID = order.getID();
    if (!send(destination, &order, sizeof(FileDeclaration)))
        return false;

    AnswerDeclaration answer(0, 0); Address sender;

    for (size_t i = 0; i < 15000000; i++)
        if (receive(sender, &answer, sizeof(AnswerDeclaration)) > 0) {
            // заменить на анализирующую функцию
            if ((sender == destination && answer.cmp_ID(my_ID) && bool(answer))) {
                // можно выделить в отдельную функцию, второстепенно
                for (size_t i = 0; i < order.getPacketAmount(); ++i) {
                    Infopacket buffer;
                    buffer.reset(file, i, my_ID);
                    send(destination, &buffer, sizeof(Infopacket));
                }
                return true;
            }
            else
                cout << "Answer hasn`t came..." << std::endl;
            break;
        }
    // сделать приём завершающей декларации или запроса
    return false;
}

bool UDPBasedTransmittion::receiveFile(FileDeclaration &order, const char *another_name) {
    FileDescriptor file(another_name == NULL ? order.getName() : another_name, "wb+");
    Infopacket buf_pack; Address sender;
    // наиболее простой массив для отслеживания дошедших пакетов
    std::vector<unsigned char> list_of_lack(order.getPacketAmount());
    for (unsigned char& i : list_of_lack) i = 0;

    size_t control_sum = order.getPacketAmount();
    for (size_t i = 0; i < 15000000 && control_sum; i++) {
        if (receive(sender, &buf_pack, sizeof(Infopacket))) {
            list_of_lack[buf_pack.getNumber()] = 1;
            UDPbuffer.push_back(buf_pack);
            --i, --control_sum;
        }
    }
    size_t j = 0;
    for (Infopacket& i : UDPbuffer) {
        file.setPacket(i.getData(), i.getDataLength(), j++);
    }
    //сделать запрос недошедших пакетов или отчёт о полном получении - вернуть завершённую декларацию
    return true;
}

bool UDPBasedTransmittion::receiveDeclaration(FileDeclaration &order, Address &sender) {   
    int size;
    // переписать центр с вызовом функций, обращающихся к пользователю
    for (size_t i = 0; i < 75000000; i++) {
        if ((size = m_socket.receive(sender, &order, sizeof(FileDeclaration)))) {
            if (size == sizeof(FileDeclaration)) {
                try {
                    AnswerDeclaration verdict(order.getID(), true);
                    send(sender, &verdict, sizeof(AnswerDeclaration));
                } catch (...) {
                    cout << "Incoming message has an unallowed format. Error.\n";
                    return false;
                }
                return true;
            }
            else {
                cout << "Incoming message has an unknown format.\n";
            }
        }
    }
    return false;
}
