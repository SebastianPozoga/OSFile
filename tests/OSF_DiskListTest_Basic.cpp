/* 
 * File:   OSF_DiskListTest_Basic.cpp
 * Author: spozoga
 *
 * Created on Dec 28, 2013, 1:32:08 AM
 */

#include <stdlib.h>
#include <iostream>
#include "OSF_DiskList.h"

//linux create dir
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Simple C++ Test Suite
 */

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

void testOSF_DiskList() {
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
        std::cout << "%TEST_FAILED% time=0 testname=testOSF_DiskList (OSF_DiskListTest_Basic) message=no persist header" << std::endl;
    }
    //2) must auto reserved cluster
    // (no cluster will be free)
    if (fileSystem->allocCluster() != NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=testOSF_DiskList (OSF_DiskListTest_Basic) message=no allocated cluster" << std::endl;
    }
    //clean (deletes)
    delete diskList;
    delete fileSystem;
    delete header;
    delete vhdd;

    //    OSF_FileSystem* fileSystem;
    //    unsigned int firstCluster;
    //    OSF_DiskList oSF_DiskList(fileSystem, firstCluster);
    //    if (true /*check result*/) {
    //        std::cout << "%TEST_FAILED% time=0 testname=testOSF_DiskList (OSF_DiskListTest_Basic) message=error message sample" << std::endl;
    //    }
}

void testOSF_DiskList2() {
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
        std::cout << "%TEST_FAILED% time=0 testname=testOSF_DiskList2 (OSF_DiskListTest_Basic) message=no persist header" << std::endl;
    }
    //2) must use exist cluster. (cluster count must no modified)
    // (no cluster will be free)
    if (fileSystem->allocCluster() != NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=testOSF_DiskList2 (OSF_DiskListTest_Basic) message=allocate error" << std::endl;
    }
    //clean (deletes)
    delete diskList;
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testUserstoryOneRecord() {
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
        std::cout << "%TEST_FAILED% time=0 testname=testUserstoryOneRecord (OSF_DiskListTest_Basic) message=no read first record" << std::endl;
    }
    //2)Current record must be first
    readedRecord->number = 0;
    diskList->current(readedRecord);
    if (readedRecord->number != record->number) {
        std::cout << "%TEST_FAILED% time=0 testname=testUserstoryOneRecord (OSF_DiskListTest_Basic) message=current record read error" << std::endl;
    }
    //2)No read more record
    readedRecord->number = 0;
    if (diskList->next(readedRecord) != NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=testUserstoryOneRecord (OSF_DiskListTest_Basic) message=next must return NULL" << std::endl;
    }
    //3)No modified when end
    if (readedRecord->number != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=testUserstoryOneRecord (OSF_DiskListTest_Basic) message=after read next with no exist element must no modified record" << std::endl;
    }
    //clean (deletes)
    delete diskList;
    delete fileSystem;
    delete header;
    delete record;
    delete readedRecord;
    delete vhdd;
}

void testUserstoryIterate() {
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
        std::cout << "%TEST_FAILED% time=0 testname=testUserstoryIterate (OSF_DiskListTest_Basic) message=no read first record" << std::endl;
        return;
    }
    do {
        if (diskList->current(record2) == NULL) {
            std::cout << "%TEST_FAILED% time=0 testname=testUserstoryIterate (OSF_DiskListTest_Basic) message=no read current record" << std::endl;
            return;
        }
        //current record must be equal
        if (record->number != record2->number) {
            std::cout << "%TEST_FAILED% time=0 testname=testUserstoryIterate (OSF_DiskListTest_Basic) message=current record and iterated record must be the same" << std::endl;
            return;
        }
        //count records
        readedRecordCount++;
    } while (diskList->next(record) != NULL);
    //2)The record count must be equal to added record number
    if (readedRecordCount != addedRecordCount) {
        std::cout << "%TEST_FAILED% time=0 testname=testUserstoryIterate (OSF_DiskListTest_Basic) message=readed record count is different than added record count" << std::endl;
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
    struct stat st = {0};
    if (stat("./OSF_test_data", &st) == -1) {
        mkdir("./OSF_test_data", 0777);
    }


    std::cout << "%SUITE_STARTING% OSF_DiskListTest_Basic" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testOSF_DiskList (OSF_DiskListTest_Basic)" << std::endl;
    testOSF_DiskList();
    std::cout << "%TEST_FINISHED% time=0 testOSF_DiskList (OSF_DiskListTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testOSF_DiskList2 (OSF_DiskListTest_Basic)" << std::endl;
    testOSF_DiskList2();
    std::cout << "%TEST_FINISHED% time=0 testOSF_DiskList2 (OSF_DiskListTest_Basic)" << std::endl;


    std::cout << "%TEST_STARTED% testUserstoryOneRecord (OSF_DiskListTest_Basic)" << std::endl;
    testUserstoryOneRecord();
    std::cout << "%TEST_FINISHED% time=0 testUserstoryOneRecord (OSF_DiskListTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testUserstoryIterate (OSF_DiskListTest_Basic)" << std::endl;
    testUserstoryIterate();
    std::cout << "%TEST_FINISHED% time=0 testUserstoryIterate (OSF_DiskListTest_Basic)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

