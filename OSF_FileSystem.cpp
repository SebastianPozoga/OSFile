
#include "OSF_FileSystem.h"
#include "OSF_DiskList.h"
#include "OSF_Directory.h"

#include <stdio.h>
#include <string.h>

OSF_FileSystem::OSF_FileSystem(OSF_VHDDInterface* vHDD, OSF_FileSystemHeader* baseHeader, OSF_SectorInt sectorsForDiskCluster) : vHDD(vHDD), sectorsForDiskCluster(sectorsForDiskCluster) {
    //Create new hard disk
    //prepare alloc list
    this->header = new OSF_FileSystemHeader;
    this->header->clusterCount = vHDD->getSectorCount() / sectorsForDiskCluster;
    this->header->allocPointer = 2;
    OSF_SCpy(this->header->systemName, "SPFileSystem v1.0");
    OSF_SCpy(this->header->diskName, baseHeader->diskName);
    this->allocList = new OSF_DiskList<OSF_FileSystemHeader, OSF_FileSystemRecord>((OSF_FileSystem*)this, this->header, 0);
    //this->rootDir->writeHeader(this->header);
    //root dir
    OSF_DirHeder dirHeader;
    this->rootDir = new OSF_Directory((OSF_FileSystem*)this, &dirHeader, 1);
}

OSF_FileSystem::OSF_FileSystem(OSF_VHDDInterface* vHDD, OSF_SectorInt sectorsForDiskCluster) : vHDD(vHDD), sectorsForDiskCluster(sectorsForDiskCluster) {
    this->allocList = new OSF_DiskList<OSF_FileSystemHeader, OSF_FileSystemRecord>((OSF_FileSystem*)this, 0);
    this->rootDir = new OSF_Directory((OSF_FileSystem*)this, 1);
    this->header = new OSF_FileSystemHeader;
    this->allocList->readHeader(header);
}

//helper

OSF_ClusterInt OSF_FileSystem::getClusterSize() {
    return vHDD->getSectorSize() * sectorsForDiskCluster;
}

//Alloc

OSF_ClusterInt OSF_FileSystem::allocCluster() {
    if (header->allocPointer < header->clusterCount) {
        return header->allocPointer++;
    }
    OSF_FileSystemRecord r;
    if(allocList->pop(&r)==NULL){
        return NULL;
    }
    return r.freeRecord;
}

void OSF_FileSystem::freeCluster(OSF_ClusterInt number) {
    OSF_FileSystemRecord r;
    r.freeRecord = number;
    allocList->push(&r);
}

OSF_ClusterInt OSF_FileSystem::read(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count) {
    return vHDD->read(clusterNumber * this->sectorsForDiskCluster, buffer, count * this->sectorsForDiskCluster);
}

OSF_ClusterInt OSF_FileSystem::write(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count) {
    return vHDD->write(clusterNumber * this->sectorsForDiskCluster, buffer, count * this->sectorsForDiskCluster);
}

OSF_FileSystem::~OSF_FileSystem() {
    allocList->writeHeader(header);
    delete header;
    delete allocList;
}

OSF_VHDDInterface* OSF_FileSystem::getVHDD() {
    return vHDD;
}

bool OSF_FileSystem::readHeader(OSF_FileSystemHeader* header){
    return this->allocList->readHeader(header)==NULL? false : true;
}

bool OSF_FileSystem::writeHeader(OSF_FileSystemHeader* header){
    return this->allocList->writeHeader(header)==NULL? false : true;
}