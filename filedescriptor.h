#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include <stdio.h>
#include <QDebug>
#include "constants.h"

class FileDescriptor
{
private:
    FILE *file;
    size_t file_size;
    char mode[4];
    char operator() (size_t ind) const;
    void pr_set(size_t ind, char buf);
public:
    FileDescriptor(const char *filename, const char *mode);
    ~FileDescriptor();
    char get(size_t ind) const;
    bool set(size_t ind, char buf);
    size_t getSize();
    size_t getPacket(char *buf, size_t number) const;
    size_t setPacket(char *buf, size_t len, size_t number);
    size_t countPackets(size_t len);
    bool isOpen() const noexcept;
    bool open(const char *filename, const char *mode);
    void close();
    operator bool() const noexcept;
};

#endif // FILEDESCRIPTOR_H
