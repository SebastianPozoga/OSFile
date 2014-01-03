/* 
 * File:   OSF_Directory.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 9:42 AM
 */

#ifndef OSF_DIRECTORY_H
#define	OSF_DIRECTORY_H


#include "OSF_DiskList.h"
#include "OSF_File.h"

using namespace std;

/*Terms*/
//*A "directory resource" mean file or sub-directory
//*A "sub-directory" mean a directory which parent is current directory

//Owners
#define OSF_OWNER unsigned int
#define OSF_GID   unsigned int

//File type flags
#define OSF_RESOURCE_TYPE_MASK 0x200
#define OSF_RESOURCE_TYPE_FILE 0x200
#define OSF_RESOURCE_TYPE_DIR  0

//File permission mask
#define OSF_RESOURCE_PERMISSION_MASK  0x1ff

//User permission: r(Read), w(Write), x(eXecute)
#define OSF_UR  1<<0
#define OSF_UW  1<<1
#define OSF_UX  1<<2

//Group permission: r(Read), w(Write), x(eXecute)
#define OSF_GR  1<<3
#define OSF_GW  1<<4
#define OSF_GX  1<<5

//Other permission: r(Read), w(Write), x(eXecute)
#define OSF_OR  1<<6
#define OSF_OW  1<<7
#define OSF_OX  1<<8

//structs
struct OSF_DirHeder;
struct OSF_DirRecord;


class OSF_Directory : : OSF_DiskList<OSF_DirHeder, OSF_DirRecord> {
    
private:
    
public:
    //Use exist
    OSF_Directory(OSF_FileSystem* fileSystem, OSF_ClusterInt firstCluster);
    
    /*Create new list*/
    OSF_Directory(OSF_FileSystem* fileSystem, OSF_DirHeder* header, OSF_ClusterInt firstCluster);
    
    /**
     * Static helper: checks whether the entry is a directory
     * @param r (entry) record to test
     * @return return true if record type is a directory
     */
    static bool isDir(OSF_DirRecord* r);

    /**
     * Static helper: checks whether the entry is a file
     * @param r record (entry) to test
     * @return return true if record type is a file
     */
    static bool isFile(OSF_DirRecord* r);

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
     * @return OSF_File* found file or NULL if file no found
     */
    OSF_File* getFile(string path);

};

#endif	/* OSF_DIRECTORY_H */

