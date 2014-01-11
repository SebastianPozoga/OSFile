/* 
 * File:   OSF_Directory.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 9:42 AM
 */

#ifndef OSF_DIRECTORY_H
#define	OSF_DIRECTORY_H

#include "OSF_Types.h"
#include "OSF_DiskList.h"
#include "OSF_FileSystemInterface.h"
#include "OSF_FileInterface.h"

using namespace std;



//structs
//struct OSF_DirHeder;
//struct OSF_DirRecord;


//MAIN DEF

class OSF_Directory : public OSF_DirectoryInterface, public OSF_DiskList<OSF_DirHeder, OSF_DirRecord> {
private:

public:
    //Use exist
    OSF_Directory(OSF_FileSystemInterface* fileSystem, OSF_ClusterInt firstCluster);

    /*Create new list*/
    OSF_Directory(OSF_FileSystemInterface* fileSystem, OSF_DirHeder* header, OSF_ClusterInt firstCluster);

    /**
     * Static helper: checks whether the entry is a directory
     * @param r (entry) record to test
     * @return return true if record type is a directory
     */
    //static bool isDir(OSF_DirRecord* r);

    /**
     * Static helper: checks whether the entry is a file
     * @param r record (entry) to test
     * @return return true if record type is a file
     */
    //static bool isFile(OSF_DirRecord* r);

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
    OSF_DirectoryInterface* getDirectory(string path);

    /**
     * Get file by path
     * (If a founded resource has a different type return NULL. 
     * 
     * @param path path to file
     * @return OSF_FileInterface* found file or NULL if file no found
     */
    OSF_FileInterface* getFile(string path);


    /**
     * Create sub-directory with name
     * 
     * @param name directory name
     * @return OSF_DirectoryInterface created directory 
     */
    OSF_DirectoryInterface* createDir(string name);

    /**
     * Create file with name
     * 
     * @param name file name
     * @return OSF_FileInterface created file 
     */
    OSF_FileInterface* createFile(string name);


    /**
     * get object to iterate on directory
     * 
     * @param name file name
     * @return OSF_FileInterface created file 
     */
    OSF_DirIterate iterate();

    /**
     * Remove file
     * 
     * @param name name of file
     * @return 
     */
    bool remove(string name);
    
    /**
     * Create path directories
     * 
     * @param name name of file
     * @return 
     */
    OSF_DirectoryInterface* mkdir(string name);

protected:
    void initRecord(string name, OSF_DirRecord* record);
};

#endif	/* OSF_DIRECTORY_H */

