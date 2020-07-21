#include "filedeclaration.h"

FileDeclaration::FileDeclaration(const char *name, const char *comment, size_t number) : packet_num(number) {
    this->ID = rand();
    strncpy(filename, name, FILENAME_MAX_LEN);
    strncpy(this->comment, comment, COMMENT_MAX_LEN);
    finish_flag = false;
}

const char* FileDeclaration::getName() const noexcept {
    return filename;
}
const char* FileDeclaration::getComment() const noexcept {
    return comment;
}
size_t FileDeclaration::getPacketAmount() const noexcept {
    return packet_num;
}

bool FileDeclaration::setComment(const void *str, size_t len) {
    if (len > COMMENT_MAX_LEN)
        return false;

    memcpy(comment, str, len);
    return true;
}

void FileDeclaration::complete() noexcept {
    finish_flag = true;
}

bool FileDeclaration::isCompleted() const noexcept {
    return finish_flag;
}

bool FileDeclaration::operator==(const FileDeclaration& decl) const noexcept {
    bool result = false;
    if ( (ID == decl.ID) && (packet_num == decl.packet_num) &&
         (!strncmp(filename, decl.filename, FILENAME_MAX)) )
        result = true;
    return result;
}

bool FileDeclaration::operator!=(const FileDeclaration& decl) const noexcept {
    return !(*this == decl);
}
