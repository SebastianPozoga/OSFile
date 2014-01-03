/* 
 * File:   OSF_DirectoryInterface.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 5:35 PM
 */

#ifndef OSF_DIRECTORYINTERFACE_H
#define	OSF_DIRECTORYINTERFACE_H

#include "OSF_Types.h"
#include "OSF_FileSystemInterface.h"
#include "OSF_FileInterface.h"

using namespace std;


//structs
struct OSF_DirHeder;
struct OSF_DirRecord;


//MAIN
class OSF_DirectoryInterface {
public:

    /**
     * ABSTRACT Static helper: checks whether the entry is a directory
     * 
     * @param r (entry) record to test
     * @return return true if record type is a directory
     */
    static bool isDir(OSF_DirRecord* r) {
        return (r->flags & OSF_RESOURCE_TYPE_MASK) == OSF_RESOURCE_TYPE_DIR;
    }

    /**
     * ABSTRACT Static helper: checks whether the entry is a file
     * 
     * @param r record (entry) to test
     * @return return true if record type is a file
     */
    static bool isFile(OSF_DirRecord* r) {
        return (r->flags & OSF_RESOURCE_TYPE_MASK) == OSF_RESOURCE_TYPE_FILE;
    }

    /**
     * Find resource record (entry) by path
     * (the function find into sub-directories)
     * 
     * @param path path of resource
     * @return resource (entry) record or NULL if resource no found
     */
    OSF_DirRecord* get(string path);
    /**
     * Helper: Find resource record (entry) by path
     * (the function find into sub-directories)
     * 
     * @param path path of resource
     * @param dir the current directory
     * @return resource (entry) record or NULL if resource no found
     */
    OSF_DirRecord* searchPath(string path, OSF_Directory* dir);

    /**
     * Get sub-directory by path
     * 
     * @param path path to directory
     * @return OSF_Dir 
     */
    OSF_Directory* getDir(string path);

    /**
     * Get file by path
     * (If a founded resource has a different type return NULL. 
     * 
     * @param path path to file
     * @return OSF_FileInterface* found file or NULL if file no found
     */
    OSF_FileInterface* getFile(string path);

};

struct OSF_DirHeder {
};

struct OSF_DirRecord {
    //Resource name 
    char name[20];
    //description of the resource (type and permissions)
    int flags;
    //first disc cluster of resource
    OSF_ClusterInt firstCluster;
    //ID of resource owner
    //(for operating system)
    OSF_OWNER owner;
    //Resource group ID
    //(for operating system)
    OSF_GID gid;
};

#endif	/* OSF_DIRECTORYINTERFACE_H */

