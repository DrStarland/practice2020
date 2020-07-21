#ifndef FILEDECLARATION_H
#define FILEDECLARATION_H

#include <memory>
#include <string.h>
#include "basepacket.h"
#include "constants.h"

class FileDeclaration : public BasePacket
{
private:
    size_t packet_num;
    size_t file_size;
    bool finish_flag;
    char filename[FILENAME_MAX_LEN];
    char comment[COMMENT_MAX_LEN];
public:
    FileDeclaration(const char *name, const char *comment, size_t number);
    FileDeclaration() = default;
    virtual ~FileDeclaration() = default;
    const char* getName() const noexcept;
    const char* getComment() const noexcept;
    bool setComment(const void *str, size_t len);
    size_t getPacketAmount() const noexcept;
    void complete() noexcept;
    bool isCompleted() const noexcept;
    // комментарий и флаг окончания не участвуют в сравнении
    bool operator==(const FileDeclaration& decl) const noexcept;
    bool operator!=(const FileDeclaration& decl) const noexcept;
};

#endif // FILEDECLARATION_H
