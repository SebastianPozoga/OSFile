
#include "OSF_Types.h"

void OSF_MemCpy(OSF_Memory dest, OSF_Memory src, OSF_MemorySize size){
    for(int i=0; i<size; i++){
        dest[i] = src[i];
    }
}

void OSF_StringCpy(OSF_Memory dest, OSF_Memory src, OSF_MemorySize size){
    for(int i=0; i<size; i++){
        dest[i] = src[i];
        if(src[i]=='\0') return;
    }
    dest[size-1] = '\0';
}

