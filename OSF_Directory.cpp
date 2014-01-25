/* 
 * File:   OSF_Directory.cpp
 * Author: spozoga
 * 
 * Created on January 3, 2014, 9:42 AM
 */

#include "OSF_Types.h"
#include "OSF_FileSystemInterface.h"
#include "OSF_DirectoryInterface.h"

#include "OSF_DiskList.h"
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

/*
 * Never return root dir
 */
OSF_DirRecord* OSF_Directory::searchPath(string path, OSF_Directory* dir) {
    OSF_DirRecord* r = new OSF_DirRecord;
    std::istringstream iss(path);
    string name;
    int lvl = 0;
    //OSF_Dir subDir = NULL;
    //iterate over path modes
    while (getline(iss, name, '/')) {
        if (name == "") {
            continue;
        }
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
        } while (dir->next(r) != NULL);
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
    if (this->getFileSystem()->allocCluster() == NULL) {
        throw OSF_Exception("No allocate cluster");
    }
    record->owner = NULL;
    OSF_scpy(record->name, name.c_str());
}

OSF_DirIterate OSF_Directory::iterate() {
    return (OSF_DirIterate)this;
}

bool OSF_Directory::remove(string name) {
    OSF_DiskList* iterate = (OSF_DiskList*)this;
    OSF_DirRecord popRecord;
    if (iterate->pop(&popRecord) == NULL) {
        //if no contain file/directory
        return false;
    };
    //if first if file/directory to delete
    if (strcmp(popRecord.name, name.c_str()) == 0) {
        remove(popRecord);
        return true;
    }
    //search
    OSF_DirRecord record;
    for (iterate->first(&record); iterate->current(&record) != NULL; iterate->next(&record)) {
        //
        if (strcmp(record.name, name.c_str()) == 0) {
            remove(record);
            iterate->currentWrite(&popRecord);
            return true;
        }
    }
    //if no exist
    iterate->push(&popRecord);
    return false;
}

void OSF_Directory::remove(OSF_DirRecord record) {
    if (OSF_DirectoryInterface::isDir(&record)) {
        OSF_DirectoryInterface* dir = new OSF_Directory(this->getFileSystem(), record.firstCluster);
        dir->free();
        delete dir;
        this->getFileSystem()->freeCluster(record.firstCluster);
    } else {
        OSF_File* file = new OSF_File(this->getFileSystem(), record.firstCluster);
        file->free();
        delete file;
        this->getFileSystem()->freeCluster(record.firstCluster);
    }
}

void OSF_Directory::free() {
    OSF_DirRecord r;
    while (this->pop(&r)) {
        if(isDir(&r)){
            OSF_Directory* dir = new OSF_Directory(this->getFileSystem(), r.firstCluster);
            dir->free();
            delete dir;
        }else{
            OSF_File* file = new OSF_File(this->getFileSystem(), r.firstCluster);
            file->free();
            delete file;
        }
    }
}

OSF_DirectoryInterface* OSF_Directory::mkdir(string path) {
    std::istringstream iss(path);
    string name;
    int lvl = 0;
    OSF_DirectoryInterface* dir = this->getFileSystem()->getRootDir();
    //iterate over path modes
    while (getline(iss, name, '/')) {
        if (name == "") {
            continue;
        }
        //find node in current directory
        //(iterate over directory)
        OSF_DirRecord* record = dir->get(name);
        OSF_DirectoryInterface* newDir = NULL;
        if (record == NULL) {
            newDir = dir->createDir(name);
        } else {
            newDir = new OSF_Directory(getFileSystem(), record->firstCluster);
            delete record;
        }
        if (lvl != 0) {
            delete dir;
        }
        ++lvl;
        dir = newDir;
    }
    return dir;
}