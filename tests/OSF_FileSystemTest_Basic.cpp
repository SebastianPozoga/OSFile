/* 
 * File:   OSF_FileSystemTest_Basic.cpp
 * Author: spozoga
 *
 * Created on Dec 27, 2013, 8:48:19 PM
 */

#include <stdlib.h>
#include <iostream>
#include "OSF_VHDD.h"
#include "OSF_FileSystem.h"

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
#define CLUSTER_SIZE SECTOR_SIZE*SECTORS_PER_CLUSTER

void testGetVHDD() {
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_FileSystem_GetVHDD.osf", (OSF_SectorSizeInt)SECTOR_SIZE, (OSF_SectorInt)SECTOR_COUNT, true);
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, (OSF_ClusterInt)SECTORS_PER_CLUSTER);
    if (fileSystem->getVHDD() != vhdd) {
        std::cout << "%TEST_FAILED% time=0 testname=testGetVHDD (OSF_FileSystemTest_Basic) message=error message sample" << std::endl;
    }
    delete fileSystem;
    delete vhdd;
}

void testOSF_FileSystem() {
    char diskName[] = "TDisk\0";
    //Create new Silesystem
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_FileSystem_FileSystem.osf", SECTOR_SIZE, SECTOR_COUNT, true);
    OSF_FileSystemHeader* header = new OSF_FileSystemHeader;
    strncpy(header->diskName, diskName, sizeof(header->diskName));
    header->diskName[ sizeof(header->diskName)-1 ] ='\n';
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, SECTORS_PER_CLUSTER);
    delete fileSystem;
    fileSystem = new OSF_FileSystem(vhdd, (OSF_ClusterInt)SECTORS_PER_CLUSTER);
    fileSystem->readHeader(header);
    //test
    if (strcmp(diskName, header->diskName)!=0) {
        std::cout << "%TEST_FAILED% time=0 testname=testGetVHDD (OSF_FileSystemTest_Basic) message=error message sample" << std::endl;
    }
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testOSF_FileSystem2() {
    //    OSF_VHDD* vHDD;
    //    int sectorsForDiskCluster;
    //    OSF_FileSystem oSF_FileSystem(vHDD, sectorsForDiskCluster);
    //    if (true /*check result*/) {
    //        std::cout << "%TEST_FAILED% time=0 testname=testOSF_FileSystem2 (OSF_FileSystemTest_Basic) message=error message sample" << std::endl;
    //    }
}

void testAllocCluster() {
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_FileSystem_testAllocCluster.osf", (OSF_SectorSizeInt)SECTOR_SIZE, (OSF_SectorInt)6, true);
    OSF_FileSystemHeader* header = new OSF_FileSystemHeader();
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, 2);
    //TEST1: one cluster is free. The first free cluster is 3
    // cluster 0 contain alocation list and file system header
    // cluster 1 contain root directory
    OSF_ClusterInt cluster = fileSystem->allocCluster();
    if (cluster != 2) {
        std::cout << "%TEST_FAILED% time=0 testname=testAllocCluster (OSF_FileSystemTest_Basic) message=no allocate first cluster" << std::endl;
    }
    //TEST2: all cluster is allocated. Return NULL
    cluster = fileSystem->allocCluster();
    if (cluster != NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=testAllocCluster (OSF_FileSystemTest_Basic) message=no return null when all cluster is allocated" << std::endl;
    }
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testFreeCluster() {
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_FileSystem_testFreeCluster.osf", SECTOR_SIZE, 6, true);
    OSF_FileSystemHeader* header = new OSF_FileSystemHeader();
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, header, 2);
    //TEST1: one cluster is free. The first free cluster is 3
    // cluster 0 contain alocation list and file system header
    // cluster 1 contain root directory
    OSF_ClusterInt cluster = fileSystem->allocCluster();
    if (cluster != 2) {
        std::cout << "%TEST_FAILED% time=0 testname=testFreeCluster (OSF_FileSystemTest_Basic) message=no allocate first cluster" << std::endl;
    }
    //TEST2: all cluster is allocated. Return NULL
    cluster = fileSystem->allocCluster();
    if (cluster != NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=testFreeCluster (OSF_FileSystemTest_Basic) message=no return null when all cluster is allocated" << std::endl;
    }
    //TEST FREE and Allocated third cluster (index=2)
    fileSystem->freeCluster(2);
    cluster = fileSystem->allocCluster();
    if (cluster != 2) {
        std::cout << "%TEST_FAILED% time=0 testname=testFreeCluster (OSF_FileSystemTest_Basic) message=no allocated free cluster" << std::endl;
    }
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testGetClusterSize() {
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_FileSystem_testGetClusterSize.osf", (OSF_SectorSizeInt)SECTOR_SIZE, (OSF_SectorInt)SECTOR_COUNT, true);
    OSF_FileSystem* fileSystem = new OSF_FileSystem(vhdd, (OSF_ClusterInt)SECTORS_PER_CLUSTER);
    if (fileSystem->getClusterSize() != SECTORS_PER_CLUSTER*SECTOR_SIZE) {
        std::cout << "%TEST_FAILED% time=0 testname=testGetClusterSize (OSF_FileSystemTest_Basic) message=no correct value" << std::endl;
    }
    delete fileSystem;
    delete vhdd;
}


void testUserstoryWriteRead() {
    OSF_ClusterInt clusterCount = 2;
    OSF_ClusterInt blockSize = clusterCount*CLUSTER_SIZE;
    //char path[] = "./OSF_test_data/OSF_FileSystem_us_writeread.osf";
    //Create default data
    char* buffer = (char*) malloc(blockSize);
    for (int i = 0; i < blockSize; i++) {
        buffer[i] = (char) (i+1)%30;
    }
    //Crete struct
    OSF_VHDD* vhdd = new OSF_VHDD("./OSF_test_data/OSF_FileSystem_us_writeread.osf", SECTOR_SIZE, SECTOR_COUNT, true);
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
        std::cout << "%TEST_FAILED% time=0 testname=testUserstoryWriteRead (OSF_FileSystemTest_Basic) message=valid error" << std::endl;
    }
    free(buffer);
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testRead() {
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
        std::cout << "%TEST_FAILED% time=0 testname=testRead (OSF_FileSystemTest_Basic) message=read error" << std::endl;
    }
    free(buffer);
    delete fileSystem;
    delete header;
    delete vhdd;
}

void testWrite() {
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
        std::cout << "%TEST_FAILED% time=0 testname=testWrite (OSF_FileSystemTest_Basic) message=no open file" << std::endl;
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
        std::cout << "%TEST_FAILED% time=0 testname=testWrite (OSF_FileSystemTest_Basic) message=error message sample" << std::endl;
    }
    free(buffer);
}


/**
 * Test if the data write are lost after second write
 */
void testDoubleWrite() {
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
        std::cout << "%TEST_FAILED% time=0 testname=testDoubleWrite (OSF_FileSystemTest_Basic) message=no open file" << std::endl;
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
        std::cout << "%TEST_FAILED% time=0 testname=testDoubleWrite (OSF_FileSystemTest_Basic) message=no valid read cluster "<<clusterNumber << std::endl;
    }
    //read2
    f = fopen(path, "rb");
    if (!f) {
        std::cout << "%TEST_FAILED% time=0 testname=testDoubleWrite (OSF_FileSystemTest_Basic) message=no open file" << std::endl;
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
        std::cout << "%TEST_FAILED% time=0 testname=testDoubleWrite (OSF_FileSystemTest_Basic) message=no valid read cluster 0" << std::endl;
    }
    //end
    free(buffer);
}

int main(int argc, char** argv) {
    struct stat st = {0};
    if (stat("./OSF_test_data", &st) == -1) {
        mkdir("./OSF_test_data", 0777);
    }
    
    std::cout << "%SUITE_STARTING% OSF_FileSystemTest_Basic" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testUserstoryWriteRead (OSF_FileSystemTest_Basic)" << std::endl;
    testUserstoryWriteRead();
    std::cout << "%TEST_FINISHED% time=0 testUserstoryWriteRead (OSF_FileSystemTest_Basic)" << std::endl;
    
    std::cout << "%TEST_STARTED% testRead (OSF_FileSystemTest_Basic)" << std::endl;
    testRead();
    std::cout << "%TEST_FINISHED% time=0 testRead (OSF_FileSystemTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testWrite (OSF_FileSystemTest_Basic)" << std::endl;
    testWrite();
    std::cout << "%TEST_FINISHED% time=0 testWrite (OSF_FileSystemTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testDoubleWrite (OSF_FileSystemTest_Basic)" << std::endl;
    testDoubleWrite();
    std::cout << "%TEST_FINISHED% time=0 testDoubleWrite (OSF_FileSystemTest_Basic)" << std::endl;
    
    std::cout << "%TEST_STARTED% testGetVHDD (OSF_FileSystemTest_Basic)" << std::endl;
    testGetVHDD();
    std::cout << "%TEST_FINISHED% time=0 testGetVHDD (OSF_FileSystemTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testOSF_FileSystem (OSF_FileSystemTest_Basic)" << std::endl;
    testOSF_FileSystem();
    std::cout << "%TEST_FINISHED% time=0 testOSF_FileSystem (OSF_FileSystemTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testOSF_FileSystem2 (OSF_FileSystemTest_Basic)" << std::endl;
    testOSF_FileSystem2();
    std::cout << "%TEST_FINISHED% time=0 testOSF_FileSystem2 (OSF_FileSystemTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testAllocCluster (OSF_FileSystemTest_Basic)" << std::endl;
    testAllocCluster();
    std::cout << "%TEST_FINISHED% time=0 testAllocCluster (OSF_FileSystemTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testFreeCluster (OSF_FileSystemTest_Basic)" << std::endl;
    testFreeCluster();
    std::cout << "%TEST_FINISHED% time=0 testFreeCluster (OSF_FileSystemTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testGetClusterSize (OSF_FileSystemTest_Basic)" << std::endl;
    testGetClusterSize();
    std::cout << "%TEST_FINISHED% time=0 testGetClusterSize (OSF_FileSystemTest_Basic)" << std::endl;


    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

