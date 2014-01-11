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
#include "OSF_DiskListInterface.h"

using namespace std;


//structs
struct OSF_DirHeder {
};

struct OSF_DirRecord {
    //Resource name 
    char name[20];
    //description of the resource (type and permissions)
    int flags;
    //first disc cluster of resource
    OSF_ClusterInt firstCluster;
};


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
    virtual OSF_DirRecord* get(string path){
        throw OSF_Exception("OSF_DirectoryInterface No implement get", 301);
    }
    
    /**
     * Helper: Find resource record (entry) by path
     * (the function find into sub-directories)
     * 
     * @param path path of resource
     * @param dir the current directory
     * @return resource (entry) record or NULL if resource no found
     */
    virtual OSF_DirRecord* searchPath(string path, OSF_DirectoryInterface* dir){
        throw OSF_Exception("OSF_DirectoryInterface No implement searchPath", 301);
    }

    /**
     * Get sub-directory by path
     * 
     * @param path path to directory
     * @return OSF_Dir 
     */
    virtual OSF_DirectoryInterface* getDirectory(string path){
        throw OSF_Exception("OSF_DirectoryInterface No implement getDir", 301);
    }

    /**
     * Get file by path
     * (If a founded resource has a different type return NULL. 
     * 
     * @param path path to file
     * @return OSF_FileInterface* found file or NULL if file no found
     */
    virtual OSF_FileInterface* getFile(string path){
        throw OSF_Exception("OSF_DirectoryInterface No implement getFile", 301);
    }

    /**
     * Create sub-directory with name
     * 
     * @param name directory name
     * @return OSF_DirectoryInterface created directory 
     */
    virtual OSF_DirectoryInterface* createDir(string name){
        throw OSF_Exception("OSF_DirectoryInterface No implement createDir", 301);
    }

    /**
     * Create file with name
     * 
     * @param name file name
     * @return OSF_FileInterface created file 
     */
    virtual OSF_FileInterface* createFile(string name){
        throw OSF_Exception("OSF_DirectoryInterface No implement createFile", 301);
    }

    /**
     * get object to iterate on directory
     * 
     * @param name file name
     * @return OSF_FileInterface created file 
     */
    virtual OSF_DirIterate iterate(){
        throw OSF_Exception("OSF_DirectoryInterface No implement iterate", 301);
    }
    
    /**
     * Remove file
     * 
     * @param name name of file
     * @return 
     */
    virtual bool remove(string name) {
        throw OSF_Exception("OSF_DirectoryInterface No implement remove", 301);
    }
};


#endif	/* OSF_DIRECTORYINTERFACE_H */

