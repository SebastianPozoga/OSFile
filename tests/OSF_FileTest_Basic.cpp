/* 
 * File:   OSF_FileTest_Basic.cpp
 * Author: spozoga
 *
 * Created on Jan 4, 2014, 12:03:21 PM
 */

#include "OSF_Types.h"
#include "OSF_TestUnit.h"
#include "OSF_FileInterface.h"
#include "OSF_File.h"

#include <stdlib.h>
#include <iostream>

void userstoryWriteAndReadOnBegin(OSF_TestUnit* testUnit) {
    //prepare
    testUnit->setErrorMsg("prepare data");
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    OSF_FileInterface* file = rootDir->createFile("myfile");
    //Story 1:
    // Write random data to file. Clean buffer. Read data. Test equals.
    // (start with first sector)
    long int clusterCount = 3;
    int bufferSize = fs->getClusterSize()*3; 
    char* buffer = testUnit->createBuffer( bufferSize );
    testUnit->setErrorMsg("write");
    file->write(buffer,0,3);
    testUnit->cleanBuffer(buffer, bufferSize);
    testUnit->setErrorMsg("read");
    file->read(buffer,0,3);
    testUnit->setErrorMsg("a readed data must be the some as write data");
    if (!testUnit->testBuffer(buffer, bufferSize)) {
        return testUnit->error();
    }
}

void userstoryWriteAndReadThird(OSF_TestUnit* testUnit) {
    //prepare
    testUnit->setErrorMsg("prepare data");
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    OSF_FileInterface* file = rootDir->createFile("myfile");
    //Story 1:
    // Write random data to file. Clean buffer. Read data. Test equals.
    // (skip firsts clusters)
    long int clusterCount = 3;
    int bufferSize = fs->getClusterSize()*3; 
    char* buffer = testUnit->createBuffer( bufferSize );
    testUnit->setErrorMsg("write");
    file->write(buffer,2,3);
    testUnit->cleanBuffer(buffer, bufferSize);
    testUnit->setErrorMsg("read");
    file->read(buffer,2,3);
    testUnit->setErrorMsg("a readed data must be the some as write data");
    if (!testUnit->testBuffer(buffer, bufferSize)) {
        return testUnit->error();
    }
}

void userstoryWriteAndReadInside(OSF_TestUnit* testUnit) {
    //prepare
    testUnit->setErrorMsg("prepare data");
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    OSF_FileInterface* file = rootDir->createFile("myfile");
    //Story 1:
    // Write random data to file. Clean buffer. Read data. Test equals.
    // (write data inside file)
    long int clusterCount = 5;
    int bufferSize = fs->getClusterSize()*clusterCount; 
    char* buffer = testUnit->createBuffer( bufferSize );
    testUnit->setErrorMsg("write the entire contents of their file");
    file->write(buffer,0,clusterCount);
    testUnit->setErrorMsg("write three sector inside file");
    file->write(buffer,1,clusterCount);
    testUnit->cleanBuffer(buffer, bufferSize);
    testUnit->setErrorMsg("read");
    file->read(buffer,1,3);
    testUnit->setErrorMsg("a readed data must be the some as write data");
    if (!testUnit->testBuffer(buffer, bufferSize)) {
        return testUnit->error();
    }
}

int main(int argc, char** argv) {

    OSF_TestUnit testUnit;

    testUnit.startTests("OSF_FileTest_Basic");

    testUnit.test("userstoryWriteAndReadOnBegin", &userstoryWriteAndReadOnBegin);
    testUnit.test("userstoryWriteAndReadThird", &userstoryWriteAndReadThird);

    testUnit.endTests();

    return (EXIT_SUCCESS);
}

