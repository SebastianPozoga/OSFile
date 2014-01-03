/* 
 * File:   SPFileSystem.h
 * Author: spozoga
 *
 * Created on December 25, 2013, 12:29 AM
 */

#ifndef SPFILESYSTEM_H
#define	SPFILESYSTEM_H

#include "OSF_Directory.h"
#include "OSF_VHDD.h"
//#include "OSF_DiskList.h"

using namespace std;

#define OSF_ClusterInt unsigned int
#define OSF_ClusterOffset unsigned int

template<class Header, class Record>
class OSF_DiskList;

struct OSF_FileSystemHeader {
    char systemName[20];
    char diskName[30];
    OSF_ClusterInt clusterCount;
    OSF_ClusterInt allocPointer;
};

struct OSF_FileSystemRecord {
    OSF_ClusterInt freeRecord;
};

class OSF_FileSystem {
private:
    OSF_VHDD* vHDD;
    int sectorsForDiskCluster;
    OSF_FileSystemHeader* header;
    OSF_DiskList<OSF_FileSystemHeader, OSF_FileSystemRecord>* allocList;
    OSF_Directory* rootDir;

public:
    OSF_FileSystem(OSF_VHDD* vHDD, OSF_FileSystemHeader* header, OSF_SectorInt sectorsForDiskCluster);

    OSF_FileSystem(OSF_VHDD* vHDD, OSF_SectorInt sectorsForDiskCluster);

    OSF_ClusterInt getClusterSize();

    OSF_ClusterInt allocCluster();

    void freeCluster(OSF_ClusterInt number);

    OSF_ClusterInt read(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count = 1);

    OSF_ClusterInt write(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count = 1);

    virtual ~OSF_FileSystem();

    OSF_VHDD* getVHDD();

    bool readHeader(OSF_FileSystemHeader* header);

    bool writeHeader(OSF_FileSystemHeader* header);


};

#include "OSF_DiskList.h"

#endif	/* SPFILESYSTEM_H */

