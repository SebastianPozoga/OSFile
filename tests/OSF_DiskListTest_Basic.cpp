/* 
 * File:   OSF_DiskListTest_Basic.cpp
 * Author: spozoga
 *
 * Created on Dec 28, 2013, 1:32:08 AM
 */

#include <stdlib.h>
#include <iostream>
#include "OSF_DiskList.h"
#include "OSF_FileSystem.h"
#include "OSF_VHDD.h"
#include "OSF_TestUnit.h"

#define SECTOR_SIZE 128
#define SECTOR_COUNT 20
#define SECTORS_PER_CLUSTER 2
#define CLUSTER_SIZE SECTOR_SIZE*SECTORS_PER_CLUSTE

struct Header {
    int number;
};

struct Record {
    int number;
};

void testOSF_DiskList(OSF_TestUnit* testUnit) {
    //We create new disk list
    // (with cluster allocated)
    int value = 0x0f0f;
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_DiskList_DiskList.osf", SECTOR_SIZE, 1, true);
    Header* header = new Header;
    header->number = value;
    OSF_FileSystemHeader fsHeader;
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, &fsHeader, 1);
    OSF_DiskList<Header, Record>* diskList = new OSF_DiskList<Header, Record>(fileSystem, header, fileSystem->allocCluster());
    //1) header must be persist (no modified)
    header->number = 0;
    diskList->readHeader(header);
    if (header->number != value) {
        testUnit->error("no persist header");
    }
    //2) must auto reserved cluster
    // (no cluster will be free)
    if (fileSystem->allocCluster() != NULL) {
        testUnit->error("no allocated cluster");
    }
    //clean (deletes)
    delete diskList;
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testOSF_DiskList2(OSF_TestUnit* testUnit) {
    //We create new disk list
    // (with cluster allocated)
    int value = 0x0f0f;
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_DiskList_DiskList2.osf", SECTOR_SIZE, 1, true);
    Header* header = new Header;
    header->number = value;
    OSF_FileSystemHeader fsHeader;
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, &fsHeader, 1);
    OSF_DiskList<Header, Record>* diskList = new OSF_DiskList<Header, Record>(fileSystem, header, fileSystem->allocCluster());
    //Close theme and open by second constructor
    OSF_ClusterInt firstCluster = diskList->getFirstCluster();
    delete diskList;
    diskList = new OSF_DiskList<Header, Record>(fileSystem, (OSF_ClusterInt) firstCluster);
    //1) header must be persist (no modified)
    header->number = 0;
    diskList->readHeader(header);
    if (header->number != value) {
        testUnit->error("no persist header");
    }
    //2) must use exist cluster. (cluster count must no modified)
    // (no cluster will be free)
    if (fileSystem->allocCluster() != NULL) {
        testUnit->error("allocate error");
    }
    //clean (deletes)
    delete diskList;
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testUserstoryOneRecord(OSF_TestUnit* testUnit) {
    //Init data
    int value = 0x0f0f;
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_DiskList_userstoryOneRecord.osf", SECTOR_SIZE, 1, true);
    Header* header = new Header;
    OSF_FileSystemHeader fsHeader;
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, &fsHeader, 1);
    OSF_DiskList<Header, Record>* diskList = new OSF_DiskList<Header, Record>(fileSystem, header, fileSystem->allocCluster());
    Record* record = new Record;
    record->number = value;
    diskList->push(record);
    //Test read first record
    //1)Must read first
    Record* readedRecord = new Record();
    diskList->first(readedRecord);
    if (readedRecord->number != record->number) {
        testUnit->error("no read first record");
    }
    //2)Current record must be first
    readedRecord->number = 0;
    diskList->current(readedRecord);
    if (readedRecord->number != record->number) {
        testUnit->error("current record read error");
    }
    //2)No read more record
    readedRecord->number = 0;
    if (diskList->next(readedRecord) != NULL) {
        testUnit->error("next must return NULL");
    }
    //3)No modified when end
    if (readedRecord->number != 0) {
        testUnit->error("after read next with no exist element must no modified record");
    }
    //clean (deletes)
    delete diskList;
    delete fileSystem;
    delete header;
    delete record;
    delete readedRecord;
    delete vhdd;
}

void testUserstoryIterate(OSF_TestUnit* testUnit) {
    //Init data
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_DiskList_userstoryIterate.osf", SECTOR_SIZE, 15, true);
    Header* header = new Header;
    OSF_FileSystemHeader fsHeader;
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, &fsHeader, 1 );
    OSF_DiskList<Header, Record>* diskList = new OSF_DiskList<Header, Record>(fileSystem, header, fileSystem->allocCluster());
    Record* record = new Record;
    //NOTE: We use SECTOR_SIZE because we want to alloc more then one sector
    int addedRecordCount = 0;
    for (int i = 0; i < SECTOR_SIZE * 2; i++) {
        record->number = i;
        diskList->push(record);
        ++addedRecordCount;
    }
    //Test read first record
    //1)Must read all record by first, next, current
    Record* record2 = new Record;
    int readedRecordCount = 0;
    if (diskList->first(record) == NULL) {
        testUnit->error("no read first record");
        return;
    }
    do {
        if (diskList->current(record2) == NULL) {
        testUnit->error("no read current record");
            return;
        }
        //current record must be equal
        if (record->number != record2->number) {
        testUnit->error("current record and iterated record must be the same");
            return;
        }
        //count records
        readedRecordCount++;
    } while (diskList->next(record) != NULL);
    //2)The record count must be equal to added record number
    if (readedRecordCount != addedRecordCount) {
        testUnit->error("readed record count is different than added record count");
        return;
    }
    //clean (deletes)
    delete diskList;
    delete fileSystem;
    delete header;
    delete record;
    delete record2;
    delete vhdd;
}

int main(int argc, char** argv) {
    OSF_TestUnit testUnit;
    
    testUnit.startTests("OSF_DiskListTest_Basic");
    
    testUnit.test("testOSF_DiskList", &testOSF_DiskList);
    testUnit.test("testOSF_DiskList2", &testOSF_DiskList2);
    testUnit.test("testUserstoryOneRecord", &testUserstoryOneRecord);
    testUnit.test("testUserstoryIterate", &testUserstoryIterate);

    testUnit.endTests();

    return (EXIT_SUCCESS);
}

