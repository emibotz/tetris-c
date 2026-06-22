#include <unistd.h>
#include "ioutils.h"

int WriteByte(int fd, char value) {
    return write(fd, &value, sizeof(char));
}

int WriteInt(int fd, int value) {
    return write(fd, &value, sizeof(int));
}

int ReadByte(int fd, char* out) {
    return read(fd, out, sizeof(char));
}

int ReadInt(int fd, int *out) {
    return read(fd, out, sizeof(int));
}
