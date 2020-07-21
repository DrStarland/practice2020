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
    if (!bool(file)) {
        return false;
    }

    FileDeclaration order(filename, "", file.countPackets(MAX_PACKET_LEN));
    int my_ID = order.getID();
    if (!send(destination, &order, sizeof(FileDeclaration))) {
        return false;
    }

    AnswerDeclaration answer(0, 0); Address sender;
    for (size_t i = 0; i < 15000000; i++)
        if (receive(sender, &answer, sizeof(AnswerDeclaration)) > 0) {
            // заменить на анализирующую функцию
            if ((sender == destination && answer.cmp_ID(my_ID) && bool(answer))) {
                // можно выделить в отдельную функцию, второстепенно
                for (size_t i = 0; i < order.getPacketAmount(); ++i) {
                    Infopacket buffer;
                    buffer.reset(file, i, my_ID);
                    if (i == 313)
                        send(destination, &buffer, sizeof(Infopacket));
                }
                break;
            }
            else
                cout << "Answer hasn`t came..." << std::endl;
            break;
        }

        // сделать приём завершающей декларации или запроса
    for (size_t over_i = 0; over_i < 4; ++over_i) {
        FileDeclaration final_order;
        int temp;
        for (size_t i = 0; i < 15000000; i++) {
            if ((temp = receive(sender, &final_order, sizeof(FileDeclaration))) > 0) {
                if (temp == sizeof(FileDeclaration) && final_order == order) {
                    if (final_order.isCompleted()) {
                        cout << "File was successful sended.\n";
                        return true;
                    }
                    else {
                        size_t control_sum = *((size_t*) final_order.getComment());
                        cout << "eto odin? " << control_sum << std::endl;
                        for (size_t i = 0; i < 15000000 && control_sum; i++) {
                            Infopacket buffer;
                            if (receive(sender, &buffer, sizeof(Infopacket))) {
                                cout << "sum packet " << control_sum << std::endl;
                                UDPbuffer.push_front(buffer);
                                --i, --control_sum;
                            }
                        }
                        for (size_t i = 0; i < *((size_t*) final_order.getComment()) - control_sum; ++i) {
                            Infopacket buf_pack = UDPbuffer.front();
                            UDPbuffer.pop_front();
                            size_t *arr = (size_t*) buf_pack.getData();
                            for (size_t j = 0; j < buf_pack.getDataLength() / sizeof(size_t); ++j) {
                                if (j % 5 == 0)
                                    cout << "///\n";
                                Infopacket buffer;
                                buffer.reset(file, arr[j], my_ID);
                                send(destination, &buffer, sizeof(Infopacket));
                            }
                        }
                        cout << "spravilis\n";
                    }
                }
                else
                    cout << "nu chto-to ushlo\n";
            }
        }
    }

    return false;
}

bool UDPBasedTransmittion::receiveFile(FileDeclaration &order, const char *another_name) {
    FileDescriptor file(another_name == NULL ? order.getName() : another_name, "wb+");
    Infopacket buf_pack; Address sender;
    // наиболее простой массив для отслеживания дошедших пакетов
    std::vector<unsigned char> list_of_packets(order.getPacketAmount());
    for (unsigned char& i : list_of_packets) i = 0;

    size_t control_sum = order.getPacketAmount();
    unsigned char stop = 4;
    bool sort_flag = false;
    while (!order.isCompleted() && stop) {
        cout << "this was working twice, control " << control_sum << std::endl;
        for (size_t i = 0; i < 150000 && control_sum; i++) {
            if (receive(sender, &buf_pack, sizeof(Infopacket))) {
                if (buf_pack.getNumber() == 228)
                    cout << "228 incoming!!\n";
                list_of_packets[buf_pack.getNumber()] = 1;
                UDPbuffer.push_back(buf_pack);
                --i, --control_sum;
            }
        }

        size_t count_of_lacks = 0;
        for (unsigned char i : list_of_packets)
            if (i == 0)
                ++count_of_lacks;

        cout << "Count of lossed packets: " << count_of_lacks << std::endl;

        if (count_of_lacks == 0) {
            order.complete();
            cout << "result sending " << send(sender, &order, sizeof(FileDeclaration)) << "\n";
            break;
        }
        else {
//            order.complete();
//            cout << "result sending " << send(sender, &order, sizeof(FileDeclaration)) << "\n";
//            break;
            cout << "!\n";
            shared_ptr<size_t[]> dop_list(new size_t[count_of_lacks]);
            {
                size_t j = 0, number = 0;
                for (unsigned char i : list_of_packets) {
                    if (i == 0)
                        dop_list[j++] = number;
                    ++number;
                }
            }
            cout << "!!\n";
            size_t amount_in_packet = MAX_PACKET_LEN / sizeof(size_t);
            size_t amount_packets = ceil((double) count_of_lacks / (double) amount_in_packet);
            order.setComment(&amount_packets, sizeof(size_t));
            send(sender, &order, sizeof(FileDeclaration));
            cout << "!!!\n";
            cout << "amount_in_packet " << amount_in_packet << std::endl;
            cout << "amount_packets " << amount_packets << " dop info " << count_of_lacks / amount_in_packet << std::endl;
            Infopacket buf_pack;
            size_t i = 0;
            for (; i < count_of_lacks / amount_in_packet; ++i) {
                buf_pack.reset(dop_list.get() + i * amount_in_packet, amount_in_packet * sizeof(size_t), i, order.getID());
                send(sender, &buf_pack, sizeof(Infopacket));
            }
            cout << "!!!!\n";
            cout << "i = " << i << std::endl;
            if (count_of_lacks % amount_in_packet != 0) {
                buf_pack.reset(dop_list.get() + i * amount_in_packet,
                               (count_of_lacks % amount_in_packet) * sizeof(size_t), i, order.getID());
                cout << (count_of_lacks % amount_in_packet) << ", " << (count_of_lacks % amount_in_packet) * sizeof(size_t) << std::endl;
                send(sender, &buf_pack, sizeof(Infopacket));
            }
            // собрать в функцию
            cout << "!!!!!\n";
            control_sum = count_of_lacks;
            sort_flag = true;
            --stop;
        }
    }

    if (stop == 0 && !order.isCompleted()) {
        UDPbuffer.clear();
        return false;
    }

    if (sort_flag)
        UDPbuffer.sort();

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
