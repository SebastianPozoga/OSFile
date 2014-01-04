/* 
 * File:   OSF_FileInterface.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 3:48 PM
 */

#ifndef OSF_FILEINTERFACE_H
#define	OSF_FILEINTERFACE_H


#include "OSF_Types.h"
#include "OSF_Exception.h"
//#include "OSF_FileSystemInterface.h"

//Defs
//struct OSF_FileHeder;
//struct OSF_FileRecord;

/**
 * Header of File
 */
struct OSF_FileHeder {
    OSF_Size fileSize;
};

/**
 * File allocation table record
 */
struct OSF_FileRecord {
    OSF_ClusterInt cluster;
};

/**
 * Interface
 * Provide: API to file access
 */
class OSF_FileInterface {
public:

    /**
     * Read a clusters of file
     * 
     * @param ptr buffer to write
     * @param firstToRead first cluster to read
     * @param count maximum number of clusters to read
     * @return buffer if success OR NULL if fail
     */
    virtual OSF_ClusterInt read(OSF_Buffer ptr, OSF_ClusterInt firstToRead, OSF_ClusterInt count){
        throw OSF_Exception("OSF_FileInterface No implement read", 301);
    }

    /**
     * Write a clusters to file
     * 
     * @param ptr buffer to read from
     * @param firstToWrite first cluster to write
     * @param count number of clusters to write
     * @return buffer if success OR NULL if fail
     */
    virtual OSF_ClusterInt write(OSF_Buffer ptr, OSF_ClusterInt firstToWrite, OSF_ClusterInt count){
        throw OSF_Exception("OSF_FileInterface No implement write", 301);
    }
    
    /**
     * Getter for header of file
     * 
     * @return OSF_FileHeder file header
     */
    virtual OSF_FileHeder getFileHeder(){
        throw OSF_Exception("OSF_FileInterface No implement getFileHeder", 301);
    }

};


#endif	/* OSF_FILEINTERFACE_H */

