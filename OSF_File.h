/* 
 * File:   SPDir.h
 * Author: spozoga
 *
 * Created on December 25, 2013, 10:52 PM
 */

#ifndef SPFILE_H
#define	SPFILE_H

#include "OSF_FileSystem.h"
#include "OSF_DiskList.h"

#define OSF_FilePointer unsigned int
#define OSF_Size unsigned int
#define OSF_Pointer void*


using namespace std;

struct OSF_FileHeder {
    OSF_Size fileSize;
};

struct OSF_FileRecord {
    OSF_ClusterInt cluster;
};

class OSF_File : OSF_DiskList<OSF_FileHeder, OSF_FileRecord> {
public:

    /**
     * Create access object for exist file 
     * 
     * @param fileSystem object represented file system
     * @param firstCluster number of first cluster of file
     */
    OSF_File(OSF_FileSystem* fileSystem, OSF_ClusterInt firstCluster);

    /**
     * Create new file
     * 
     * @param fileSystem object represented file system
     * @param init header of file
     */
    OSF_File(OSF_FileSystem* fileSystem, OSF_FileHeder* header, OSF_ClusterInt firstCluster);

    virtual void initNew();

    OSF_ClusterInt read(OSF_Pointer ptr, OSF_ClusterInt firstToRead, OSF_ClusterInt count);

    OSF_ClusterInt write(OSF_Pointer ptr, OSF_ClusterInt firstToWrite, OSF_ClusterInt count);


private:
    OSF_FilePointer pointer;

};


#endif	/* SPFILE_H */

