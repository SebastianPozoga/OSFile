/* 
 * File:   SPDir.h
 * Author: spozoga
 *
 * Created on December 25, 2013, 10:52 PM
 */

#ifndef SPFILE_H
#define	SPFILE_H

#include "OSF_Types.h"
#include "OSF_FileInterface.h"
#include "OSF_FileSystemInterface.h"
#include "OSF_DiskList.h"


using namespace std;

class OSF_File : public OSF_FileInterface, protected OSF_DiskList<OSF_FileHeder, OSF_FileRecord> {
private:
    OSF_FilePointer pointer;
    
public:

    /**
     * Create access object for exist file 
     * 
     * @param fileSystem object represented file system
     * @param firstCluster number of first cluster of file
     */
    OSF_File(OSF_FileSystemInterface* fileSystem, OSF_ClusterInt firstCluster);

    /**
     * Create new file
     * 
     * @param fileSystem object represented file system
     * @param init header of file
     */
    OSF_File(OSF_FileSystemInterface* fileSystem, OSF_FileHeder* header, OSF_ClusterInt firstCluster);

    /**
     * Virtual function run when create new file
     */
    virtual void initNew();

    /*
     *  File Interface
     */
    OSF_ClusterInt read(OSF_Buffer ptr, OSF_ClusterInt firstToRead, OSF_ClusterInt count);
    OSF_ClusterInt write(OSF_Buffer ptr, OSF_ClusterInt firstToWrite, OSF_ClusterInt count);
    
    OSF_FileHeder* readFileHeder(OSF_FileHeder*);
    OSF_FileHeder* writeFileHeder(OSF_FileHeder*);

    void free();
};

#endif	/* SPFILE_H */

