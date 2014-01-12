
#include "OSF_Types.h"

#include <iostream>

void OSF_MemoryCopy(OSF_Memory des, OSF_Memory source, unsigned int size) {
    unsigned i = 0;
    for (; i < size; i++) {
        des[i] = source[i];
    }
    std::cout<<"size to cpy: "<<size;
}

void OSF_StrCopy(OSF_Memory des, OSF_Memory source, unsigned int size) {
    unsigned i = 0;
    for (; i < size; i++) {
        des[i] = source[i];
        if (source[i] == '\0') break;
    }
    des[i] = '\0';
    std::cout<<"size to cpy: "<<size;
}