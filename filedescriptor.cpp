#include "filedescriptor.h"
#include <cmath>
#include <iostream>

bool FileDescriptor::isOpen() const noexcept { return file != NULL; }

FileDescriptor::FileDescriptor(const char *filename, const char *mode) {
    this->file = fopen(filename, mode);
    strncpy(this->mode, mode, 4);
    file_size = getSize();
}

size_t FileDescriptor::getSize() {
    if (isOpen()) {
        size_t temp = ftell(file);
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, temp, SEEK_SET);
    }
    return file_size;
}

char FileDescriptor::get(size_t ind) const {
    char buf = 0;
    if (*this) {
        if (ind < file_size && !strncmp(this->mode, "wb", 3)) {
            fseek(file, ind, SEEK_SET);
            fread(&buf, sizeof (buf), 1, file);
            return true;
        }
    }
    //else
        //throw
    return false;
}
bool FileDescriptor::set(size_t ind, char buf) {
    if (*this) {
        if (ind < file_size && !strncmp(this->mode, "rb", 3)) {
            fseek(file, ind * sizeof(buf), SEEK_SET);
            fwrite(&buf, sizeof (buf), 1, file);
        }
    }
    //else
        //throw
    return buf;
}

char FileDescriptor::operator() (size_t ind) const {
    char buf;
    fseek(file, ind, SEEK_SET);
    fread(&buf, sizeof (buf), 1, file);
    //else
        //throw
    return buf;
}
void FileDescriptor::pr_set(size_t ind, char buf) {
    fseek(file, ind * sizeof(buf), SEEK_SET);
    fwrite(&buf, sizeof (buf), 1, file);
}

size_t FileDescriptor::getPacket(char *buf, size_t number) const {
    fseek(file, number * MAX_PACKET_LEN, SEEK_SET);
    int temp = fread(buf, 1, MAX_PACKET_LEN, file);
    //std::cout << file_size << ", " << (size_t) MAX_PACKET_LEN << ", " << temp << std::endl;
    return temp;
}

size_t FileDescriptor::setPacket(char *buf, size_t len, size_t number) {
    fseek(file, number * MAX_PACKET_LEN, SEEK_SET);
    return fwrite(buf, 1, len, file);
}

size_t FileDescriptor::countPackets(size_t len) {
    return (size_t) ceil((double) file_size / (double) len);
}

FileDescriptor::~FileDescriptor() {
    fclose(file);
}

FileDescriptor::operator bool() const noexcept { return (file != NULL); }
