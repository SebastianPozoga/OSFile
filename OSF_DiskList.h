/* 
 * File:   DiskList.h
 * Author: spozoga
 *
 * Created on December 25, 2013, 1:20 AM
 */

#ifndef DISKLIST_H
#define	DISKLIST_H

#include "OSF_Types.h"
#include "OSF_FileSystemInterface.h"
#include "OSF_DiskListInterface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/*
 * Cluster build:
 * 
 * <class header>
 * DiskStackHeader
 * <array[] class record]
 * 
 */

template<class Header, class Record>
class OSF_DiskList : public OSF_DiskListInterface<Header, Record> {
private:
    //File system data
    OSF_FileSystemInterface* fileSystem;
    OSF_ClusterInt firstCluster;
    //Current meta data
    OSF_ClusterInt currentCluster;
    //iteration
    int iterationCluster;
    int iterationRecord;
    //data
    char* buffer;
    OSF_DiskListHeader* clusterHeader;
    Header* header;
    OSF_ClusterInt recordPerCluster;
    Record* clusterRecords;

public:

    /**
     * Create object represented exist structure.
     * 
     * @param fileSystem handle to file system
     * @param firstCluster number of first cluster of the stucture
     */
    OSF_DiskList(OSF_FileSystemInterface* fileSystem, OSF_ClusterInt firstCluster) : fileSystem(fileSystem), firstCluster(firstCluster) {
        //init buffor
        OSF_ClusterInt clusterSize = fileSystem->getClusterSize();
        this->buffer = OSF_allocMemory(clusterSize);
        //load data
        this->_loadCluster(firstCluster);
        //init
        this->init();
    }

    /**
     * Init after create structure object
     */
    virtual void init() {
    }

    /**
     * Create new object
     * 
     * @param fileSystem handle to file system
     * @param header header of the object
     * @param firstCluster first cluster on disk
     */
    OSF_DiskList(OSF_FileSystemInterface* fileSystem, Header* header, OSF_ClusterInt firstCluster) : fileSystem(fileSystem), firstCluster(firstCluster) {
        //init buffor
        OSF_MemorySize clusterSize = this->fileSystem->getClusterSize();
        this->buffer = OSF_allocMemory(clusterSize);
        //create new
        this->initCluster(this->firstCluster);
        if (header != NULL) {
            this->_writeHeader(header);
        }
        //init & persist
        this->persist();
        this->initNew();
        this->persist();
        this->init();
    }

    virtual void initNew() {
    }

    Header* readHeader(Header* header) {
        //Header* header = malloc(sizeof (Header));
        this->loadCluster(this->firstCluster);
        if (this->header == NULL) {
            throw OSF_Exception("no load first cluster");
        }
        OSF_cpy(header, this->header, sizeof(Header));
        return header;
    }

    Header* writeHeader(Header* header) {
        this->loadCluster(firstCluster);
        if (this->header == NULL) {
            throw OSF_Exception("no load first cluster");
        }
        _writeHeader(header);
    }

    Header* _writeHeader(Header* header) {
        OSF_cpy(this->header, header, sizeof(Header));
        this->persist();
        return header;
    }

    /*Stack*/
    Record* push(Record* r) {
        //go to last
        this->loadCluster(firstCluster);
        while (this->clusterHeader->nextCluster != NULL) {
            this->loadCluster(this->clusterHeader->nextCluster);
        }
        if (this->recordPerCluster <= this->clusterHeader->recordsCount) {
            //alloc
            OSF_ClusterInt newClusterNumber = this->getFileSystem()->allocCluster();
            OSF_ClusterInt prevClusterNumber = this->currentCluster;
            //update current cluster metadata
            this->clusterHeader->nextCluster = newClusterNumber;
            this->persist();
            //create new
            //this->loadCluster(newClusterNumber);
            this->initCluster(newClusterNumber);
            this->clusterHeader->prevCluster = prevClusterNumber;
            //this->persist();
        }
        //add record
        Record* newRecord = &this->clusterRecords[this->clusterHeader->recordsCount];
        OSF_cpy(newRecord, r, sizeof (Record));
        ++this->clusterHeader->recordsCount;
        this->persist();
        return r;
    }

    Record* pop(Record* r) {
        //go to last
        this->loadCluster(this->firstCluster);
        while (this->clusterHeader->nextCluster != NULL) {
            this->loadCluster(this->clusterHeader->nextCluster);
        }
        if (this->clusterHeader->recordsCount == 0) {
            if (this->firstCluster == this->currentCluster) {
                return NULL;
            }
            OSF_ClusterInt prevCluster = this->clusterHeader->prevCluster;
            //remove cluster
            this->getFileSystem()->freeCluster(this->currentCluster);
            //update prev
            this->loadCluster(prevCluster);
            this->clusterHeader->nextCluster = NULL;
            //this->persist();
        }
        //add record
        --this->clusterHeader->recordsCount;
        Record* pointer = &this->clusterRecords[this->clusterHeader->recordsCount];
        OSF_cpy(r, pointer, sizeof (Record));
        this->persist();
        return r;
    }

    /*Iteration*/
    Record* first(Record* r) {
        this->iterationCluster = this->firstCluster;
        this->iterationRecord = 0;
        return this->current(r);
    }

    Record* current(Record* r) {
        this->loadCluster(this->iterationCluster);
        if (this->iterationRecord >= this->clusterHeader->recordsCount) {
            return NULL;
        }
        OSF_cpy(r, &this->clusterRecords[this->iterationRecord], sizeof (Record));
        return r;
    }

    Record* currentWrite(Record* r) {
        this->loadCluster(this->iterationCluster);
        if (this->iterationRecord >= this->clusterHeader->recordsCount) {
            return NULL;
        }
        OSF_cpy(&this->clusterRecords[this->iterationRecord], r, sizeof (Record));
        persist();
        return r;
    }

    Record* next(Record* r) {
        this->loadCluster(this->iterationCluster);
        if (this->iterationRecord >= this->clusterHeader->recordsCount
                && this->clusterHeader->nextCluster == NULL) {
            return NULL;
        }
        ++this->iterationRecord;
        if (this->current(r) == NULL) {
            if (this->clusterHeader->nextCluster == NULL) {
                return NULL;
            }
            this->iterationCluster = this->clusterHeader->nextCluster;
            this->iterationRecord = 0;
            return this->current(r);
        };
        return r;
    }

    /*Persist*/
    void persist() {
        this->getFileSystem()->write(this->currentCluster, this->buffer);
    }

    virtual ~OSF_DiskList() {
        OSF_freeMemory(this->buffer);
    }

    OSF_FileSystemInterface* getFileSystem() const {
        return this->fileSystem;
    }

    OSF_ClusterInt getFirstCluster() const {
        return firstCluster;
    }


protected:

    Header* getHeaderPtr() const {
        return header;
    }


private:

    void loadCluster(OSF_ClusterInt clusterNumber) {
        if (clusterNumber == this->currentCluster) {
            return;
        }
        _loadCluster(clusterNumber);
    }

    void _loadCluster(OSF_ClusterInt clusterNumber) {
        //read
        unsigned int readed = this->getFileSystem()->read(clusterNumber, this->buffer);
        if (readed != 1) {
            throw OSF_Exception("No read cluster");
        }
        //update meta information
        this->initCluesterMetadata(clusterNumber);
        //prevent infinity loop (when no reload correctly) 
        if (clusterNumber!=0 && this->clusterHeader->nextCluster == clusterNumber) {
            throw new OSF_Exception("load cluster error", 505);
        }
    }

    void initCluster(OSF_ClusterInt clusterNumber) {
        //init pointers
        this->initCluesterMetadata(clusterNumber);
        //Counters
        //this->clusterHeader->clusterNumber = clusterNumber;
        this->clusterHeader->recordsCount = 0;
        //offsets
        /*if (clusterNumber == this->firstCluster) {
            this->clusterHeader->dataOffset = sizeof (OSF_DiskListHeader) + sizeof (Header);
        } else {
            this->clusterHeader->dataOffset = sizeof (OSF_DiskListHeader);
        }
        //Sizes
        OSF_ClusterOffset dataSize = this->fileSystem->getClusterSize() - this->clusterHeader->dataOffset;
        this->clusterHeader->recordPerCluster = dataSize / sizeof (Record);*/
        //List pointers
        this->clusterHeader->nextCluster = 0;
        this->clusterHeader->prevCluster = 0;
    }

    void initCluesterMetadata(OSF_ClusterInt currentCluster) {
        this->currentCluster = currentCluster;
        this->clusterHeader = (OSF_DiskListHeader*) this->buffer;
        OSF_ClusterOffset dataOffset;
        if (currentCluster == this->firstCluster) {
            dataOffset = sizeof (OSF_DiskListHeader) + sizeof (Header);
            this->header = (Header*) (&this->buffer[ sizeof (OSF_DiskListHeader) ]);
            this->clusterRecords = (Record*) (&buffer[dataOffset]);
        } else {
            dataOffset = sizeof (OSF_DiskListHeader);
            this->header = NULL;
            this->clusterRecords = (Record*) (&buffer[ sizeof (OSF_DiskListHeader) ]);
        }
        this->recordPerCluster = (this->fileSystem->getClusterSize() - dataOffset) / sizeof (Record);
    }
};

#endif	/* DISKLIST_H */

