/* 
 * File:   OSF_PlainAPI.h
 * Author: spozoga
 *
 * Created on January 8, 2014, 12:49 PM
 */

#ifndef OSF_PLAINAPI_H
#define	OSF_PLAINAPI_H

#include "OSF_Types.h"
#include "OSF_VHDD.h"
#include "OSF_FileSystem.h"

#include <string>

using namespace std;

struct OSF_PlainAPIData {
    OSF_VHDD* vHDD;
    OSF_FileSystem* fs;
};

class OSF_FileHandle {
private:
    OSF_FileInterface* file;

public:
    OSF_PlainAPIInt pointer;

    OSF_FileHandle(OSF_FileInterface* file) : file(file) {
        this->pointer = 0;
    }

    ~OSF_FileHandle() {
        delete this->file;
    }

    OSF_FileInterface* getFile() const {
        return this->file;
    }
};

OSF_FileHandle* OSF_Open(string path);

void OSF_Close(OSF_FileHandle* fileHandle);

OSF_PlainAPIData* OSF_PlainAPI_init();

OSF_PlainAPIInt OSF_Read(OSF_FileHandle* fileHandle, void* buf, OSF_PlainAPIInt count);

OSF_PlainAPIInt OSF_Write(OSF_FileHandle* fileHandle, void* buf, OSF_PlainAPIInt count);

void OSF_Ls(string path, void (*callback)(OSF_DirRecord*));

OSF_OWNER OSF_chownFile(string path);

void OSF_chownFile(OSF_OWNER owner, string path);

OSF_PERMISSION OSF_chmodFile(string path);

void OSF_chmodFile(OSF_PERMISSION permission, string path);

void OSF_remove(string dir, string filename);

#endif	/* OSF_PLAINAPI_H */

