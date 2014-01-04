/* 
 * File:   OSF_Directory.cpp
 * Author: spozoga
 * 
 * Created on January 3, 2014, 9:42 AM
 */

#include "OSF_Types.h"
#include "OSF_FileSystemInterface.h"
#include "OSF_DirectoryInterface.h"
#include "OSF_Directory.h"
#include "OSF_File.h"

#include <iostream>
#include <sstream>
#include <string>

/*Create object represented exist dir*/
OSF_Directory::OSF_Directory(OSF_FileSystemInterface* fileSystem, OSF_ClusterInt firstCluster) : OSF_DiskList(fileSystem, firstCluster) {
}

/*Create new dir*/
OSF_Directory::OSF_Directory(OSF_FileSystemInterface* fileSystem, OSF_DirHeder* header, OSF_ClusterInt firstCluster) : OSF_DiskList(fileSystem, header, firstCluster) {
}

/*bool OSF_Directory::isDir(OSF_DirRecord* r) {
    return (r->flags & OSF_RESOURCE_TYPE_MASK) == OSF_RESOURCE_TYPE_DIR;
}

bool OSF_Directory::isFile(OSF_DirRecord* r) {
    return (r->flags & OSF_RESOURCE_TYPE_MASK) == OSF_RESOURCE_TYPE_FILE;
}*/

OSF_DirRecord* OSF_Directory::get(string path) {
    return searchPath(path, this);
}

OSF_DirRecord* OSF_Directory::searchPath(string path, OSF_Directory* dir) {
    OSF_DirRecord* r = new OSF_DirRecord;
    std::istringstream iss(path);
    string name;
    int lvl = 0;
    //OSF_Dir subDir = NULL;
    //iterate over path modes
    while (getline(iss, name, '/')) {
        //find node in current directory
        //(iterate over directory)
        dir->first(r);
        bool findSubDir = false;
        do {
            if (strcmp(r->name, name.c_str())) continue;
            //if node is sub-directory find next node into
            if (isDir(r)) {
                if (lvl != 0) delete dir;
                ++lvl;
                dir = new OSF_Directory(this->getFileSystem(), r->firstCluster);
                findSubDir = true;
                break;
            }
            //if node is file return them
            return r;
        } while (this->next(r) != NULL);
        //if node by name no found return NULL
        if (findSubDir == false) {
            if (lvl != 0) delete dir;
            delete r;
            return NULL;
        }
    }
    //no more path node
    //return finded dir
    if (lvl == 0) {
        delete r;
        return NULL;
    }
    return r;
}

OSF_DirectoryInterface* OSF_Directory::getDirectory(string path) {
    OSF_DirRecord* r = this->get(path);
    if (r == NULL) {
        return NULL;
    }
    if (!isDir(r)) {
        delete r;
        return NULL;
    }
    OSF_Directory* dir = new OSF_Directory(this->getFileSystem(), (OSF_ClusterInt) r->firstCluster);
    delete r;
    return dir;
}

OSF_FileInterface* OSF_Directory::getFile(string path) {
    OSF_DirRecord* r = this->get(path);
    if (r == NULL) {
        return NULL;
    }
    if (!isFile(r)) {
        delete r;
        return NULL;
    }
    OSF_File* file = new OSF_File(this->getFileSystem(), (OSF_ClusterInt) r->firstCluster);
    delete r;
    return file;
}

OSF_DirectoryInterface* OSF_Directory::createDir(string name) {
    //prepare record
    OSF_DirHeder header;
    OSF_DirRecord record;
    initRecord(name, &record);
    record.flags = OSF_RESOURCE_TYPE_DIR | ((~OSF_RESOURCE_TYPE_MASK) & record.flags);
    OSF_Directory* dir = new OSF_Directory(getFileSystem(), &header, record.firstCluster);
    this->push(&record);
    return (OSF_DirectoryInterface*) dir;
}

OSF_FileInterface* OSF_Directory::createFile(string name) {
    //prepare record
    OSF_FileHeder header;
    header.fileSize = 0;
    OSF_DirRecord record;
    initRecord(name, &record);
    record.flags = OSF_RESOURCE_TYPE_FILE | ((~OSF_RESOURCE_TYPE_MASK) & record.flags);
    OSF_FileInterface* file = new OSF_File(getFileSystem(), &header, record.firstCluster);
    this->push(&record);
    return (OSF_FileInterface*) file;
};

void OSF_Directory::initRecord(string name, OSF_DirRecord* record) {
    record->flags = OSF_DEFAULT_PERMISSION;
    record->firstCluster = this->getFileSystem()->allocCluster();
    strncpy(record->name, name.c_str(), sizeof (record->name));
    record->name[ sizeof (record->name) - 1 ] = 0;
}

OSF_DirIterate OSF_Directory::iterate() {
    return (OSF_DirIterate)this;
}