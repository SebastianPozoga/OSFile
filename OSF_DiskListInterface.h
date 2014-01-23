/* 
 * File:   DiskList.h
 * Author: spozoga
 *
 * Created on December 25, 2013, 1:20 AM
 */

#ifndef DISKLISTINTERFACE_H
#define	DISKLISTINTERFACE_H

#include "OSF_Types.h"
#include "OSF_FileSystemInterface.h"

using namespace std;

struct OSF_DiskListHeader {
    //OSF_ClusterInt clusterNumber;
    OSF_ClusterInt prevCluster;
    OSF_ClusterInt nextCluster;
    OSF_ClusterInt recordsCount;
    //OSF_ClusterInt recordPerCluster;
    //OSF_ClusterOffset dataOffset;
};

class OSF_FileSystemInterface;

template<class Header, class Record>
class OSF_DiskListInterface {
public:

    virtual Header* readHeader(Header* header){
        throw OSF_Exception("OSF_DiskList No implement readHeader", 301);
    }

    virtual Header* writeHeader(Header* header){
        throw OSF_Exception("OSF_DiskList No implement writeHeader", 301);
    }

    virtual Record* push(Record* r){
        throw OSF_Exception("OSF_DiskList No implement push", 301);
    }

    virtual Record* first(Record* r){
        throw OSF_Exception("OSF_DiskList No implement first", 301);
    }

    virtual Record* current(Record* r){
        throw OSF_Exception("OSF_DiskList No implement current", 301);
    }

    virtual Record* next(Record* r){
        throw OSF_Exception("OSF_DiskList No implement next", 301);
    }
    
    virtual void persist(){
        throw OSF_Exception("OSF_DiskList No implement persist", 301);
    }

    virtual OSF_FileSystemInterface* getFileSystem(){
        throw OSF_Exception("OSF_DiskList No implement getFileSystem", 301);
    }

    virtual OSF_ClusterInt getFirstCluster(){
        throw OSF_Exception("OSF_DiskList No implement getFirstCluster", 301);
    }

    virtual ~OSF_DiskListInterface(){
    }

};

#endif	/* DISKLIST_H */

