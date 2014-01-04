/* 
 * File:   OSF_FileSystemTest_Basic.cpp
 * Author: spozoga
 *
 * Created on Dec 27, 2013, 8:48:19 PM
 */

#include "OSF_Types.h"
#include "OSF_TestUnit.h"
#include "OSF_VHDD.h"
#include "OSF_FileSystem.h"
#include "OSF_File.h"

#include <stdlib.h>
#include <iostream>

#define SECTOR_SIZE 128
#define SECTOR_COUNT 20
#define SECTORS_PER_CLUSTER 2
#define CLUSTER_SIZE SECTOR_SIZE*SECTORS_PER_CLUSTER

void testGetVHDD(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("no return VHDD");
    //init
    OSF_VHDD* vhdd = new OSF_VHDD(testUnit->filePath("OSF_FileSystem_GetVHDD.osf"), SECTOR_SIZE, SECTOR_COUNT, true);
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, (OSF_ClusterInt)SECTORS_PER_CLUSTER);
    //test
    if (fileSystem->getVHDD() != vhdd) {
        testUnit->error();
    }
    //destroy
    delete fileSystem;
    delete vhdd;
}

void testOSF_FileSystem(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("recognized by readed disk name is incorrect");
    //prepare
    char diskName[] = "TDisk\0";
    //Create new Silesystem
    OSF_VHDD* vhdd = new OSF_VHDD(testUnit->filePath("OSF_FileSystem_FileSystem.osf"), SECTOR_SIZE, SECTOR_COUNT, true);
    OSF_FileSystemHeader* header = new OSF_FileSystemHeader;
    strncpy(header->diskName, diskName, sizeof(header->diskName));
    header->diskName[ sizeof(header->diskName)-1 ] ='\n';
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, SECTORS_PER_CLUSTER);
    delete fileSystem;
    fileSystem = new OSF_FileSystem(vhdd, (OSF_ClusterInt)SECTORS_PER_CLUSTER);
    fileSystem->readHeader(header);
    //test
    if (strcmp(diskName, header->diskName)!=0) {
        testUnit->error();
    }
    //destroy
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testOSF_FileSystem2(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("construct for exist structure");
    OSF_VHDD vHDD(testUnit->filePath("OSF_FileSystem_FileSystem.osf"), SECTOR_SIZE, SECTOR_COUNT, true);
    OSF_FileSystem* fileSystem = new OSF_FileSystem(&vHDD, SECTOR_COUNT);
    if (!fileSystem) {
        testUnit->error();
    }
    delete fileSystem;
}

void testAllocCluster(OSF_TestUnit* testUnit) {
    OSF_VHDD* vhdd = new OSF_VHDD(testUnit->filePath("OSF_FileSystem_testAllocCluster.osf"), (OSF_SectorSizeInt)SECTOR_SIZE, (OSF_SectorInt)6, true);
    OSF_FileSystemHeader* header = new OSF_FileSystemHeader();
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, 2);
    //TEST1: one cluster is free. The first free cluster is 3
    // cluster 0 contain alocation list and file system header
    // cluster 1 contain root directory
    OSF_ClusterInt cluster = fileSystem->allocCluster();
    if (cluster != 2) {
        testUnit->error("no allocate first cluster");
    }
    //TEST2: all cluster is allocated. Return NULL
    cluster = fileSystem->allocCluster();
    if (cluster != NULL) {
        testUnit->error("no return null when all cluster is allocated");
    }
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testFreeCluster(OSF_TestUnit* testUnit) {
    OSF_VHDD* vhdd = new OSF_VHDD(testUnit->filePath("OSF_FileSystem_testFreeCluster.osf"), SECTOR_SIZE, 6, true);
    OSF_FileSystemHeader* header = new OSF_FileSystemHeader();
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, 2);
    //TEST1: one cluster is free. The first free cluster is 3
    // cluster 0 contain alocation list and file system header
    // cluster 1 contain root directory
    OSF_ClusterInt cluster = fileSystem->allocCluster();
    if (cluster != 2) {
        testUnit->error("no allocate first cluster");
    }
    //TEST2: all cluster is allocated. Return NULL
    cluster = fileSystem->allocCluster();
    if (cluster != NULL) {
        testUnit->error("no return null when all cluster is allocated");
    }
    //TEST FREE and Allocated third cluster (index=2)
    fileSystem->freeCluster(2);
    cluster = fileSystem->allocCluster();
    if (cluster != 2) {
        testUnit->error("no allocated free cluster");
    }
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testGetClusterSize(OSF_TestUnit* testUnit) {
    OSF_VHDD* vhdd = new OSF_VHDD(testUnit->filePath("OSF_FileSystem_testGetClusterSize.osf"), (OSF_SectorSizeInt)SECTOR_SIZE, (OSF_SectorInt)SECTOR_COUNT, true);
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, (OSF_ClusterInt)SECTORS_PER_CLUSTER);
    if (fileSystem->getClusterSize() != SECTORS_PER_CLUSTER*SECTOR_SIZE) {
        testUnit->error("no correct value");
    }
    delete fileSystem;
    delete vhdd;
}


void testUserstoryWriteRead(OSF_TestUnit* testUnit) {
    OSF_ClusterInt clusterCount = 2;
    OSF_ClusterInt blockSize = clusterCount*CLUSTER_SIZE;
    //char path[] = "./OSF_test_data/OSF_FileSystem_us_writeread.osf";
    //Create default data
    char* buffer = (char*) malloc(blockSize);
    for (int i = 0; i < blockSize; i++) {
        buffer[i] = (char) (i+1)%30;
    }
    //Crete struct
    OSF_VHDD* vhdd = new OSF_VHDD(testUnit->filePath("OSF_FileSystem_us_writeread.osf"), SECTOR_SIZE, SECTOR_COUNT, true);
    OSF_FileSystemHeader* header =new OSF_FileSystemHeader;
    strncpy((char*) &header->diskName, "TDisk", sizeof (header->diskName));
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, SECTORS_PER_CLUSTER);
    //write & read
    OSF_ClusterInt cint = 3;
    fileSystem->write(cint, buffer, clusterCount);
    memset(buffer, 0, blockSize);
    fileSystem->read(cint, buffer, clusterCount);
    //test
    bool valid = true;
    for (int i = 0; i < blockSize; i++) {
        if (buffer[i] != (char) (i+1)%30) {
            valid = false;
            break;
        }
    }
    //test
    if (!valid) {
        testUnit->error("data valid error");
    }
    free(buffer);
    delete fileSystem;
    delete header;
    delete vhdd;
}

void userstoryWriteAndReadOnBegin(OSF_TestUnit* testUnit) {
    //prepare data
    OSF_ClusterInt clusterCount = 2;
    OSF_ClusterInt blockSize = clusterCount*CLUSTER_SIZE;
    char path[] = "./OSF_test_data/OSF_FileSystem_read.osf";
    //Create default data
    char* buffer = (char*) malloc(blockSize);
    for (int i = 0; i < blockSize; i++) {
        buffer[i] = (char) i;
    }
    //file write
    FILE* f = fopen(path, "w");
    fseek(f, CLUSTER_SIZE * 3, SEEK_SET);
    fwrite(buffer, 1, blockSize, f);
    fclose(f);
    //Crete struct
    OSF_VHDD* vhdd = new OSF_VHDD(path, SECTOR_SIZE, SECTOR_COUNT);
    OSF_FileSystemHeader* header =new OSF_FileSystemHeader;
    strncpy((char*) &header->diskName, "TDisk", sizeof (header->diskName));
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, SECTORS_PER_CLUSTER);
    //write
    fileSystem->read(3, buffer, clusterCount);
    //test
    bool valid = true;
    for (int i = 0; i < blockSize; i++) {
        if (buffer[i] != (char) i) {
            valid = false;
            break;
        }
    }
    //report
    if (!valid) {
        testUnit->error("valid readed data error");
    }
    free(buffer);
    delete fileSystem;
    delete header;
    delete vhdd;
}

void userstoryWriteAndReadThird(OSF_TestUnit* testUnit) {
    //prepare data
    OSF_ClusterInt clusterCount = 2;
    OSF_ClusterInt blockSize = clusterCount*CLUSTER_SIZE;
    char path[] = "./OSF_test_data/OSF_FileSystem_write.osf";
    //Crete struct
    OSF_VHDD* vhdd = new OSF_VHDD(path, SECTOR_SIZE, SECTOR_COUNT, true);
    OSF_FileSystemHeader* header =new OSF_FileSystemHeader;
    strncpy((char*) &header->diskName, "TestDisk", sizeof (header->diskName));
    //header->diskName[sizeof (header->diskName) - 1] = '\0';

    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, SECTORS_PER_CLUSTER);
    //Create default data
    char* buffer = (char*) malloc(blockSize);
    for (int i = 0; i < blockSize; i++) {
        buffer[i] = (char) (i+1)%30;
    }
    //write
    OSF_ClusterInt clusterNumber = 3;
    fileSystem->write(clusterNumber, buffer, clusterCount);
    delete fileSystem;
    delete header;
    delete vhdd;
    //read
    FILE* f = fopen(path, "r");
    if (!f) {
        testUnit->error("no open file");
        return;
    }
    fseek(f, CLUSTER_SIZE * clusterNumber, SEEK_SET);
    fread(buffer, 1, blockSize, f);
    fclose(f);
    //test
    bool valid = true;
    for (int i = 0; i < blockSize; i++) {
        if (buffer[i] != (char) (i+1)%30) {
            valid = false;
            break;
        }
    }
    //report
    if (!valid) {
        testUnit->error("valid data error");
    }
    free(buffer);
}


/**
 * Test if the data write are lost after second write
 */
void testDoubleWrite(OSF_TestUnit* testUnit) {
    OSF_ClusterInt clusterCount = 2;
    OSF_ClusterInt blockSize = clusterCount*CLUSTER_SIZE;
    char path[] = "./OSF_test_data/OSF_FileSystem_doubleWrite.osf";
    //Crete struct
    OSF_VHDD* vhdd = new OSF_VHDD(path, SECTOR_SIZE, SECTOR_COUNT, true);
    OSF_FileSystemHeader* header =new OSF_FileSystemHeader;
    strncpy((char*) &header->diskName, "TestDisk", sizeof (header->diskName));
    //header->diskName[sizeof (header->diskName) - 1] = '\0';

    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, SECTORS_PER_CLUSTER);
    //Create default data
    char* buffer = (char*) malloc(blockSize);
    for (int i = 0; i < blockSize; i++) {
        buffer[i] = (char) (i+1)%30;
    }
    //write
    OSF_ClusterInt clusterNumber = 3;
    //first
    fileSystem->write(clusterNumber, buffer, clusterCount);
    fileSystem->write((clusterNumber+1), buffer, clusterCount);
    //delete
    delete fileSystem;
    delete header;
    delete vhdd;
    //read
    FILE* f = fopen(path, "rb");
    if (!f) {
        testUnit->error("no open file");
        return;
    }
    fseek(f, CLUSTER_SIZE * clusterNumber, SEEK_SET);
    memset(buffer, 0, blockSize);
    int readed1 = fread(buffer, 1, blockSize, f);
    fclose(f);
    //test
    bool valid = true;
    for (int i = 0; i < blockSize; i++) {
        if (buffer[i] != (char) (i+1)%30) {
            valid = false;
            break;
        }
    }
    if (!valid) {
        testUnit->error("no valid readed data");
    }
    //read2
    f = fopen(path, "rb");
    if (!f) {
        testUnit->error("no open file");
        return;
    }
    fseek(f, CLUSTER_SIZE *(clusterNumber+1), SEEK_SET);
    memset(buffer, 0, blockSize);
    int readed2 = fread(buffer, 1, blockSize, f);
    fclose(f);
    //test2
    valid = true;
    for (int i = 0; i < blockSize; i++) {
        if (buffer[i] != (char) (i+1)%30) {
            valid = false;
            break;
        }
    }
    if (!valid) {
        testUnit->error("no valid read cluster");
    }
    //end
    free(buffer);
}

int main(int argc, char** argv) {
    
    OSF_TestUnit testUnit;
    
    testUnit.startTests("OSF_FileSystemTest_Basic");
    
    testUnit.test("testUserstoryWriteRead", &testUserstoryWriteRead);
    testUnit.test("testRead", &userstoryWriteAndReadOnBegin);
    testUnit.test("testWrite", &userstoryWriteAndReadThird);
    testUnit.test("testDoubleWrite", &testDoubleWrite);
    testUnit.test("testGetVHDD", &testGetVHDD);
    testUnit.test("testOSF_FileSystem", &testOSF_FileSystem);
    testUnit.test("testOSF_FileSystem2", &testOSF_FileSystem2);
    testUnit.test("testAllocCluster", &testAllocCluster);
    testUnit.test("testFreeCluster", &testFreeCluster);
    testUnit.test("testGetClusterSize", &testGetClusterSize);

    testUnit.endTests();

    return (EXIT_SUCCESS);
}

