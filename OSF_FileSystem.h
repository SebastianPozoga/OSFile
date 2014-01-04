/* 
 * File:   SPFileSystem.h
 * Author: spozoga
 *
 * Created on December 25, 2013, 12:29 AM
 */

#ifndef SPFILESYSTEM_H
#define	SPFILESYSTEM_H

#include "OSF_Types.h"
#include "OSF_VHDDInterface.h"
#include "OSF_FileSystemInterface.h"
#include "OSF_DiskList.h"

//#include "OSF_VHDD.h"
//#include "OSF_DiskList.h"

//

class OSF_Directory;
//struct OSF_FileSystemHeader;
//struct OSF_FileSystemRecord;


using namespace std;

class OSF_FileSystem : public OSF_FileSystemInterface {
private:
    OSF_VHDDInterface* vHDD;
    int sectorsForDiskCluster;
    OSF_FileSystemHeader* header;
    OSF_DiskList<OSF_FileSystemHeader, OSF_FileSystemRecord>* allocList;
    OSF_Directory* rootDir;

public:
    OSF_FileSystem(OSF_VHDDInterface* vHDD, OSF_FileSystemHeader* header, OSF_SectorInt sectorsForDiskCluster);

    OSF_FileSystem(OSF_VHDDInterface* vHDD, OSF_SectorInt sectorsForDiskCluster);

    OSF_ClusterInt getClusterSize();

    OSF_ClusterInt allocCluster();

    void freeCluster(OSF_ClusterInt number);

    OSF_ClusterInt read(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count = 1);

    OSF_ClusterInt write(OSF_ClusterInt clusterNumber, char* buffer, OSF_ClusterInt count = 1);

    virtual ~OSF_FileSystem();

    OSF_VHDDInterface* getVHDD();

    bool readHeader(OSF_FileSystemHeader* header);

    bool writeHeader(OSF_FileSystemHeader* header);

    OSF_DirectoryInterface* getRootDir() {
        return (OSF_DirectoryInterface*)rootDir;
    }

};

#include "OSF_Directory.h"

#endif	/* SPFILESYSTEM_H */

