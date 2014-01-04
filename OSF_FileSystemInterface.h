/* 
 * File:   OSF_FileSystemInterface.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 2:24 PM
 */

#ifndef OSF_FILESYSTEMINTERFACE_H
#define	OSF_FILESYSTEMINTERFACE_H

#include "OSF_Types.h"
#include "OSF_VHDDInterface.h"
#include "OSF_Exception.h"
#include "OSF_DirectoryInterface.h"

struct OSF_FileSystemRecord;
struct OSF_FileSystemHeader;

class OSF_FileSystemInterface {
public:
    virtual OSF_ClusterInt getClusterSize(){
        throw OSF_Exception("OSF_FileSystemInterface No implement getClusterSize", 301);
    }

    virtual OSF_ClusterInt allocCluster(){
        throw OSF_Exception("OSF_FileSystemInterface No implement allocCluster", 301);
    }

    virtual void freeCluster(OSF_ClusterInt number){
        throw OSF_Exception("OSF_FileSystemInterface No implement freeCluster", 301);
    }

    virtual OSF_ClusterInt read(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count = 1){
        throw OSF_Exception("OSF_FileSystemInterface No implement read", 301);
    }

    virtual OSF_ClusterInt write(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count = 1){
        throw OSF_Exception("OSF_FileSystemInterface No implement write", 301);
    }

    virtual OSF_VHDDInterface* getVHDD(){
        throw OSF_Exception("OSF_FileSystemInterface No implement getVHDD", 301);
    }

    virtual bool readHeader(OSF_FileSystemHeader* header){
        throw OSF_Exception("OSF_FileSystemInterface No implement readHeader", 301);
    }

    virtual bool writeHeader(OSF_FileSystemHeader* header){
        throw OSF_Exception("OSF_FileSystemInterface No implement writeHeader", 301);
    }
    
    virtual OSF_DirectoryInterface* getRootDir(){
        throw OSF_Exception("OSF_FileSystemInterface No implement getRootDir", 301);
    }

    virtual ~OSF_FileSystemInterface(){
    }
};

struct OSF_FileSystemHeader {
    char systemName[20];
    char diskName[30];
    OSF_ClusterInt clusterCount;
    OSF_ClusterInt allocPointer;
};

struct OSF_FileSystemRecord {
    OSF_ClusterInt freeRecord;
};


#endif	/* OSF_FILESYSTEMINTERFACE_H */

