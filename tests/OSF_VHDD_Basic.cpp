/* 
 * File:   OSF_VHDD_Basic.cpp
 * Author: spozoga
 *
 * Created on Dec 27, 2013, 2:58:30 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "OSF_VHDD.h"

//linux create dir
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Simple C++ Test Suite
 */

#define SECTOR_SIZE 512

void testOSF_VHDD() {
    char path[] = "./OSF_test_data/OSF_VHDD_construct.osf";
    OSF_VHDD vhdd(path, SECTOR_SIZE, 1, true);
    //Test exist
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=testOSF_VHDD (OSF_VHDD_Basic) message=error message sample" << std::endl;
    }
}

void testRead() {
    char path[] = "./OSF_test_data/OSF_VHDD_read.osf";
    int sectorCount = 2;
    int blockSize = SECTOR_SIZE*sectorCount;
    char* buffer = (char*) malloc(blockSize);
    for (int i = 0; i < blockSize; i++) {
        buffer[i] = (char) i;
    }
    FILE* f = fopen(path, "w");
    fseek(f, SECTOR_SIZE, SEEK_SET);
    fwrite(buffer, 1, blockSize, f);
    fclose(f);
    //Init VHDD 
    OSF_VHDD vhdd(path, SECTOR_SIZE, 3);
    //read
    memset(buffer, 0, blockSize);
    vhdd.read(1, buffer, sectorCount);
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
        std::cout << "%TEST_FAILED% time=0 testname=testRead (OSF_VHDD_Basic) message=error message sample" << std::endl;
    }
    free(buffer);
}

void testWrite() {
    char path[] = "./OSF_test_data/OSF_VHDD_write.osf";
    //Init VHDD 
    OSF_VHDD vhdd(path, SECTOR_SIZE, 2, true);
    //Create default data
    char* buffer = (char*) malloc(SECTOR_SIZE);
    for (int i = 0; i < SECTOR_SIZE; i++) {
        buffer[i] = (char) i;
    }
    //write
    vhdd.write(1, buffer, 1);
    //read
    FILE* f = fopen(path, "r");
    fseek(f, SECTOR_SIZE, SEEK_SET);
    fread(buffer, 1, SECTOR_SIZE, f);
    fclose(f);
    //test
    bool valid = true;
    for (int i = 0; i < SECTOR_SIZE; i++) {
        if (buffer[i] != (char) i) {
            valid = false;
            break;
        }
    }
    //report
    if (!valid) {
        std::cout << "%TEST_FAILED% time=0 testname=testRead (OSF_VHDD_Basic) message=error message sample" << std::endl;
    }
    free(buffer);
}

int main(int argc, char** argv) {
    struct stat st = {0};
    if (stat("./OSF_test_data", &st) == -1) {
        mkdir("./OSF_test_data", 0777);
    }

    std::cout << "%SUITE_STARTING% OSF_VHDD_Basic" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testOSF_VHDD (OSF_VHDD_Basic)" << std::endl;
    testOSF_VHDD();
    std::cout << "%TEST_FINISHED% time=0 testOSF_VHDD (OSF_VHDD_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testRead (OSF_VHDD_Basic)" << std::endl;
    testRead();
    std::cout << "%TEST_FINISHED% time=0 testRead (OSF_VHDD_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testWrite (OSF_VHDD_Basic)" << std::endl;
    testWrite();
    std::cout << "%TEST_FINISHED% time=0 testWrite (OSF_VHDD_Basic)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

