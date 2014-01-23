/* 
 * File:   OSF_VHDD_Basic.cpp
 * Author: spozoga
 *
 * Created on Dec 27, 2013, 2:58:30 PM
 */

#include "OSF_TestUnit.h"
#include "OSF_VHDD.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#define SECTOR_SIZE 512

void testOSF_VHDD(OSF_TestUnit* testUnit) {
    char path[] = "./OSF_test_data/OSF_VHDD_construct.osf";
    OSF_VHDD vhdd(path, SECTOR_SIZE, 1, true);
    //Test exist
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        testUnit->error("no create file");
    }
}

void userstoryWriteAndReadOnBegin(OSF_TestUnit* testUnit) {
    char path[] = "./OSF_test_data/OSF_VHDD_read.osf";
    int sectorCount = 2;
    int blockSize = SECTOR_SIZE*sectorCount;
    OSF_Memory buffer = OSF_allocMemory(blockSize);
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
        testUnit->error("read error");
    }
    OSF_freeMemory(buffer);
}

void userstoryWriteAndReadThird(OSF_TestUnit* testUnit) {
    char path[] = "./OSF_test_data/OSF_VHDD_write.osf";
    //Init VHDD 
    OSF_VHDD vhdd(path, SECTOR_SIZE, 2, true);
    //Create default data
    OSF_Memory buffer = OSF_allocMemory(SECTOR_SIZE);
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
        testUnit->error("write error");
    }
    OSF_freeMemory(buffer);
}

int main(int argc, char** argv) {
    //Testing
    OSF_TestUnit* testUnit = new OSF_TestUnit;
    testUnit->startTests("OSF_VHDD_Basic");
    
    testUnit->test("OSF_VHDD", &testOSF_VHDD);
    testUnit->test("Read", &userstoryWriteAndReadOnBegin);
    testUnit->test("Write", &userstoryWriteAndReadThird);
    
    testUnit->endTests();
    return (EXIT_SUCCESS);
}

