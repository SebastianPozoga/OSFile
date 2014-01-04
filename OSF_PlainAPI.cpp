
#ifndef OSF_PLAINAPI_CPP
#define	OSF_PLAINAPI_CPP

#include "OSF_Types.h"
#include "OSF_VHDD.h"
#include "OSF_FileSystem.h"
#include "OSF_DirectoryInterface.h"
#include "OSF_FileInterface.h"
#include "OSF_Directory.h"
#include "OSF_File.h"

#include <iostream>
#include <string.h>

#define plainAPIDataPath "./VHDD.osf"
#define plainAPIDataSectorSize 128
#define plainAPIDataSectorCount 128
#define plainAPIDataSectorPerCluster 2
#define plainAPIDataFileClean false

#define OSF_PlainAPIInt long int

// Data for plain API adapter

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

using namespace std;

//Init plain API data

OSF_PlainAPIData* OSF_PlainAPI_init() {
    static OSF_PlainAPIData* plainAPIData = NULL;
    if (!plainAPIData) {
        plainAPIData->vHDD = new OSF_VHDD(plainAPIDataPath,
                plainAPIDataSectorSize,
                plainAPIDataSectorCount,
                false);
        plainAPIData->fs = new OSF_FileSystem(plainAPIData->vHDD, plainAPIDataSectorPerCluster);
    }
    return plainAPIData;
}

//Open File

OSF_FileHandle* OSF_Open(string path) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_FileInterface* file = plain->fs->getRootDir()->getFile(path);
    return new OSF_FileHandle(file);
}

void OSF_Close(OSF_FileHandle* fileHandle) {
    delete fileHandle;
}

OSF_PlainAPIInt OSF_Read(OSF_FileHandle* fileHandle, void* buf, OSF_PlainAPIInt count) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_PlainAPIInt clusterSize = plain->fs->getClusterSize();
    //offset
    OSF_PlainAPIInt offsetCluster = fileHandle->pointer / clusterSize;
    OSF_PlainAPIInt offsetBytes = fileHandle->pointer % clusterSize;
    //size
    OSF_PlainAPIInt clusterCount = (offsetBytes + count) / clusterSize;
    //read
    OSF_PlainAPIInt tmpSize = clusterCount*clusterSize;
    char* tmp = new char[tmpSize];
    OSF_PlainAPIInt readed = fileHandle->getFile()->read(tmp, offsetCluster, clusterCount);
    readed = readed * clusterSize - offsetCluster;
    memcpy(buf, &tmp[offsetBytes], count);
    delete tmp;
    return readed;
}

OSF_PlainAPIInt OSF_Write(OSF_FileHandle* fileHandle, void* buf, OSF_PlainAPIInt count) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_PlainAPIInt clusterSize = plain->fs->getClusterSize();
    //offset
    OSF_PlainAPIInt offsetCluster = fileHandle->pointer / clusterSize;
    OSF_PlainAPIInt offsetBytes = fileHandle->pointer % clusterSize;
    //size
    OSF_PlainAPIInt clusterCount = (offsetBytes + count) / clusterSize;
    //read
    OSF_PlainAPIInt tmpSize = clusterCount*clusterSize;
    char* tmp = new char[tmpSize];
    fileHandle->getFile()->read(tmp, offsetCluster, clusterCount);
    memcpy(&tmp[offsetBytes], buf, count);
    fileHandle->getFile()->write(tmp, offsetCluster, clusterCount);
    delete tmp;
    return count;
}

void OSF_Ls(string path, void (*callback)(OSF_DirRecord*)) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_DirectoryInterface* dir = plain->fs->getRootDir()->getDirectory(path);
    if(!dir){
        throw OSF_Exception("OSF_Ls: directory no found",303);
    }
    OSF_DirIterate iterate = dir->iterate();
    OSF_DirRecord rec;
    for(iterate->first(&rec); iterate->current(&rec)!=NULL; iterate->next(&rec)){
        callback(&rec);
    }
    delete dir;
}

OSF_OWNER OSF_chownFile(string path) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_FileInterface* file = plain->fs->getRootDir()->getFile(path);
    OSF_FileHeder header;
    file->readFileHeder(&header);
    delete file;
    return header.owner;
}

void OSF_chownFile(string path, OSF_OWNER owner) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_FileInterface* file = plain->fs->getRootDir()->getFile(path);
    OSF_FileHeder header;
    file->readFileHeder(&header);
    header.owner = owner;
    delete file;
    file->writeFileHeder(&header);
}

OSF_PERMISSION OSF_chmodFile(string path) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_FileInterface* file = plain->fs->getRootDir()->getFile(path);
    OSF_FileHeder header;
    file->readFileHeder(&header);
    delete file;
    return header.permission;
}

void OSF_chmodFile(string path, OSF_PERMISSION permission) {
    OSF_PlainAPIData* plain = OSF_PlainAPI_init();
    OSF_FileInterface* file = plain->fs->getRootDir()->getFile(path);
    OSF_FileHeder header;
    file->readFileHeder(&header);
    header.permission = permission;
    file->writeFileHeder(&header);
    delete file;
}

#endif	/* OSF_PLAINAPI_CPP */