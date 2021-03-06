/* 
 * File:   OSF_PlainAPITest_Basic.cpp
 * Author: spozoga
 *
 * Created on Jan 8, 2014, 12:43:55 PM
 */

#include "OSF_Types.h"
#include "OSF_PlainAPI.h"
#include "OSF_TestUnit.h"

#include <stdlib.h>
#include <iostream>

OSF_PlainAPIData* _initPlainAPI(OSF_TestUnit* testUnit) {
    OSF_PlainAPIData* plainAPIData = OSF_PlainAPI_init();
    delete plainAPIData->fs;
    delete plainAPIData->vHDD;
    plainAPIData->vHDD = new OSF_VHDD(testUnit->filePath("PlainAPI"), 128, 80, true);
    OSF_FileSystemHeader header;
    OSF_scpy(header.diskName, "testDisk");
    plainAPIData->fs = new OSF_FileSystem(plainAPIData->vHDD, &header, 2);
    //insert default data
    //directories
    OSF_DirectoryInterface* rootDir = plainAPIData->fs->getRootDir();
    OSF_DirectoryInterface* d1 = rootDir->createDir("d1");
    OSF_DirectoryInterface* d2_1 = d1->createDir("d2_1");
    OSF_DirectoryInterface* d2_2 = d2_1->createDir("d2_2");
    OSF_DirectoryInterface* d3 = d2_1->createDir("d3");
    OSF_DirectoryInterface* d4 = d3->createDir("d4");
    //files
    OSF_FileInterface* f1 = d1->createFile("f1");
    //trite data
    int bufSize = 128 * 2;
    char* buf = testUnit->createBuffer(bufSize);
    f1->write(buf, 0, 2);
    delete buf;
    //clean
    delete d1, d2_1, d2_2, d3, d4, f1;
    return plainAPIData;
}

void testOSF_Open(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("can not open file");
    OSF_FileHandle* handle = OSF_Open("d1/f1");
    if (!handle) {
        testUnit->error();
    }
}

void testOSF_Close(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("can not open file");
    OSF_FileHandle* handle = OSF_Open("d1/f1");
    if (!handle) {
        testUnit->error();
    }
    testUnit->setErrorMsg("close error");
    OSF_Close(handle);
}

void testOSF_WriteAndRead(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("can not open file");
    OSF_FileHandle* handle = OSF_Open("d1/f1");
    if (!handle) {
        testUnit->error();
    }
    char* buf = new char[128];
    testUnit->setErrorMsg("write error");
    //Write
    testUnit->writeBufferTestData(buf, sizeof(buf));
    OSF_Write(handle, buf, sizeof(buf));
    testUnit->cleanBuffer(buf, sizeof(buf));
    //Read
    handle->pointer = 0;
    testUnit->setErrorMsg("read error");
    OSF_Read(handle, buf, sizeof(buf));
    if (!testUnit->testBuffer(buf, sizeof(buf))) {
        testUnit->error();
    };
    testUnit->setErrorMsg("close error");
    OSF_Close(handle);
}

/*void testOSF_Write(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("can not open file");
    OSF_FileHandle* handle = OSF_Open("d1/f1");
    if (!handle) {
        testUnit->error();
    }
    testUnit->setErrorMsg("write error");
    char* buf = new char[128];
    testUnit->cleanBuffer(buf, 128);
    OSF_Write(handle, buf, 128);
    memset(buf, '0', 128);
    handle->getFile()->read(buf, 0, 128);
    if (!testUnit->testBuffer(buf, 128, 64)) {
        testUnit->error();
    };
    testUnit->setErrorMsg("close error");
    OSF_Close(handle);
}*/

struct TestOSF_LsCountData {
    int count;
} testOSF_LsCountData;

void testOSF_LsCount(OSF_DirRecord* r){
    testOSF_LsCountData.count++;
}

void testOSF_Ls(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("listing error");
    OSF_Ls("d1", testOSF_LsCount);
    if(testOSF_LsCountData.count!=2){
        return testUnit->error();
    }
}

void testOSF_chownFile(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("write chown error");
    int owner = 0xFAF;
    OSF_chownFile(owner, "d1/f1");
    testUnit->setErrorMsg("read chown error");
    int readerOwner = OSF_chownFile("d1/f1");
    if(owner!=readerOwner){
        testUnit->error();
    }
}

void testOSF_chmodFile(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("write chmod error");
    int mod = OSF_ALL_PERMISSION;
    OSF_chownFile(mod, "d1/f1");
    testUnit->setErrorMsg("read chmod error");
    int readerMod = OSF_chownFile("d1/f1");
    if(mod!=readerMod){
        testUnit->error();
    }
}

void testOSF_Remove(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    testUnit->setErrorMsg("delete error");
    OSF_remove("d1/f1");
    testUnit->setErrorMsg("file was not deleted");
    OSF_PlainAPIData* plainAPIData = OSF_PlainAPI_init();
    OSF_DirRecord* r = plainAPIData->fs->getRootDir()->get("d1/f1");
    if(r!=NULL){
        testUnit->error();
    }
    delete r;
}

void testOSF_Mkdir(OSF_TestUnit* testUnit) {
    testUnit->setErrorMsg("Init test");
    _initPlainAPI(testUnit);
    OSF_PlainAPIData* plainAPIData = OSF_PlainAPI_init();
    testUnit->setErrorMsg("delete error");
    string path = "n1/n2/n3";
    OSF_mkdir(path);
    testUnit->setErrorMsg("directories was not create");
    //OSF_PlainAPIData* plainAPIData = OSF_PlainAPI_init();
    OSF_DirectoryInterface* dir = plainAPIData->fs->getRootDir()->getDirectory(path);
    if(dir==NULL){
        testUnit->error();
    }
    delete dir;
}

int main(int argc, char** argv) {
    OSF_TestUnit testUnit;

    //system("ulimit -c unlimited");
    
    testUnit.startTests("OSF_PlainAPITest_Basic");
    
    testUnit.test("testOSF_LsCount", &testOSF_Ls);
    testUnit.test("testOSF_chownFile", &testOSF_chownFile);
    testUnit.test("testOSF_chmodFile", &testOSF_chmodFile);
    testUnit.test("testOSF_Remove", &testOSF_Remove);
    testUnit.test("testOSF_Mkdir", &testOSF_Mkdir);
    
    testUnit.test("testOSF_Open", &testOSF_Open);
    testUnit.test("testOSF_Close", &testOSF_Close);
    testUnit.test("testOSF_WriteAndRead", &testOSF_WriteAndRead);
    //testUnit.test("testOSF_Write", &testOSF_Write);

    testUnit.endTests();

    return (EXIT_SUCCESS);
}

