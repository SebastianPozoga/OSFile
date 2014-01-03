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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct OSF_DiskListHeader {
    OSF_ClusterInt clusterNumber;
    OSF_ClusterInt prevCluster;
    OSF_ClusterInt nextCluster;
    OSF_ClusterInt recordsCount;
    OSF_ClusterInt recordPerCluster;
    OSF_ClusterOffset dataOffset;
};

/*
 * Cluster build:
 * 
 * <class header>
 * DiskStackHeader
 * <array[] class record]
 * 
 */

template<class Header, class Record>
class OSF_DiskList {
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
    Record* clusterRecords;

public:

    /*Load list*/
    OSF_DiskList(OSF_FileSystemInterface* fileSystem, OSF_ClusterInt firstCluster) : fileSystem(fileSystem), firstCluster(firstCluster) {
        //init buffor
        OSF_ClusterInt clusterSize = fileSystem->getClusterSize();
        this->buffer = (char*) malloc(clusterSize);
        this->clusterHeader = (OSF_DiskListHeader*) buffer;
        //load data
        this->loadCluster(firstCluster);
        //init
        this->init();
    }

    virtual void init() {
    }

    /*Create new list*/
    OSF_DiskList(OSF_FileSystemInterface* fileSystem, Header* header, OSF_ClusterInt firstCluster) : fileSystem(fileSystem) {
        //init buffor
        this->buffer = (char*) malloc(this->fileSystem->getClusterSize());
        this->clusterHeader = (OSF_DiskListHeader*) buffer;
        //create new
        this->firstCluster = firstCluster;
        this->initClusterHeader(this->firstCluster);
        if (header != NULL) {
            this->writeHeader(header);
        }
        //init & persist
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
            return NULL;
        }
        memcpy(header, this->header, sizeof (Header));
        return header;
    }

    Header* writeHeader(Header* header) {
        this->loadCluster(firstCluster);
        if (this->header == NULL) {
            return NULL;
        }
        memcpy(this->header, header, sizeof (Header));
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
        if (this->clusterHeader->recordPerCluster <= this->clusterHeader->recordsCount) {
            //register new cluster
            OSF_ClusterInt prevClusterNumber = this->currentCluster;
            OSF_ClusterInt newClusterNumber = this->getFileSystem()->allocCluster();
            //add next cluster data
            this->clusterHeader->nextCluster = newClusterNumber;
            this->persist();
            //init new cluster
            this->loadCluster(newClusterNumber);
            this->initClusterHeader(newClusterNumber);
            this->clusterHeader->prevCluster = prevClusterNumber;
            this->persist();
        }
        //add record
        Record* newRecord = &this->clusterRecords[this->clusterHeader->recordsCount];
        memcpy(newRecord, r, sizeof (Record));
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
            //remove cluster
            OSF_ClusterInt prevCluster = this->clusterHeader->prevCluster;
            this->getFileSystem()->freeCluster(this->currentCluster);
            this->loadCluster(prevCluster);
            //this->clusterHeader->nextCluster = NULL;
        }
        //add record
        this->clusterHeader->nextCluster = NULL; //never has next 
        --this->clusterHeader->recordsCount;
        Record* pointer = &this->clusterRecords[this->clusterHeader->recordsCount];
        memcpy(r, pointer, sizeof (Record));
        this->persist();
        return r;
    }

    /*Iteration*/
    Record* first(Record* r) {
        this->iterationCluster = this->firstCluster;
        this->iterationRecord = 0;
        //this->loadCluster(firstCluster);
        return this->current(r);
    }

    Record* current(Record* r) {
        this->loadCluster(this->iterationCluster);
        if (this->iterationRecord >= this->clusterHeader->recordsCount) {
            return NULL;
            //this->loadCluster(this->clusterHeader->nextCluster);
            //this->iterationRecord = 0;
        }
        memcpy(r, &this->clusterRecords[this->iterationRecord], sizeof (Record));
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
            if (this->clusterHeader->nextCluster != NULL) {
                this->iterationCluster = this->clusterHeader->nextCluster;
                this->iterationRecord = 0;
                return this->current(r);
            } else {
                return NULL;
            }
        };
        return r;
    }

    /*Persist*/
    void persist() {
        this->getFileSystem()->write(this->currentCluster, this->buffer);
    }

    virtual ~OSF_DiskList() {
        free(this->buffer);
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
        this->currentCluster = clusterNumber;
        this->getFileSystem()->read(clusterNumber, this->buffer);
        this->initDiskListPointers();
    }

    OSF_ClusterInt initClusterHeader(OSF_ClusterInt clusterNumber) {
        //init pointers
        this->initDiskListPointers();
        //Counters
        this->clusterHeader->clusterNumber = clusterNumber;
        this->clusterHeader->recordsCount = 0;
        //offsets
        if (clusterNumber == this->firstCluster) {
            this->clusterHeader->dataOffset = sizeof (OSF_DiskListHeader) + sizeof (Header);
        } else {
            this->clusterHeader->dataOffset = sizeof (OSF_DiskListHeader);
        }
        //Sizes
        OSF_ClusterOffset dataSize = this->fileSystem->getClusterSize() - this->clusterHeader->dataOffset;
        this->clusterHeader->recordPerCluster = dataSize / sizeof (Record);
        //List pointers
        this->clusterHeader->nextCluster = NULL;
        this->clusterHeader->prevCluster = NULL;
    }

    OSF_ClusterInt initDiskListPointers() {
        this->clusterHeader = (OSF_DiskListHeader*) this->buffer;
        if (this->clusterHeader->dataOffset != sizeof (OSF_DiskListHeader)) {
            this->header = (Header*) (&this->buffer[ sizeof (OSF_DiskListHeader) ]);
        } else {
            this->header = NULL;
        }
        this->clusterRecords = (Record*) (&buffer[ clusterHeader->dataOffset ]);
    }
};

#endif	/* DISKLIST_H */

