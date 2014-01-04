/* 
 * File:   OSF_DirectoryTest_Basic.cpp
 * Author: spozoga
 *
 * Created on Jan 3, 2014, 9:13:10 PM
 */

#include "OSF_TestUnit.h"
#include "OSF_VHDD.h"
#include "OSF_FileSystem.h"
#include "OSF_Directory.h"
#include "OSF_File.h"
#include "OSF_FileInterface.h"

#include <stdlib.h>
#include <iostream>


void userstoryCreateAndGetFileRecord(OSF_TestUnit* testUnit) {
    //Story:
    // Create a file. Add to root directory. Next
    // get a directory record of the file. 
    //---
    //test:
    testUnit->setErrorMsg("create directory");
    string fileName = "my name";
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    testUnit->setErrorMsg("create file");
    rootDir->createFile(fileName);
    testUnit->setErrorMsg("get record");
    OSF_DirRecord* record = rootDir->get(fileName);
    if(strcmp(record->name, fileName.c_str())!=0){
        testUnit->error();
        return;
    }
}

void userstoryGetFile(OSF_TestUnit* testUnit) {
    //Story:
    // Create a file. Add to root directory. Next
    // get a file object. 
    //---
    //test:
    testUnit->setErrorMsg("create directory");
    string fileName = "my name";
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    testUnit->setErrorMsg("create file");
    rootDir->createFile(fileName);
    testUnit->setErrorMsg("get file");
    OSF_FileInterface* file = rootDir->getFile(fileName);
    testUnit->setErrorMsg("file no exist");
    if(!file){
        return testUnit->error();
    }
}

void userstoryGetDirectory(OSF_TestUnit* testUnit) {
    //Story:
    // Create a directory. Add to root directory. Next
    // get a directory object. 
    //---
    //test:
    testUnit->setErrorMsg("create directory");
    string resourceName = "my name";
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    testUnit->setErrorMsg("create file");
    rootDir->createDir(resourceName);
    testUnit->setErrorMsg("get directory");
    OSF_DirectoryInterface* dir = rootDir->getDirectory(resourceName);
    testUnit->setErrorMsg("directory no exist");
    if(!dir){
        return testUnit->error();
    }
}

void userstoryCreateAndTestByIsDir(OSF_TestUnit* testUnit) {
    //prepare
    testUnit->setErrorMsg("create directory");
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    //Story 1:
    // Create a directory. Add to root directory. Next
    // get and test a directory record. 
    string dirName = "directory name";
    testUnit->setErrorMsg("create directory");
    rootDir->createDir(dirName);
    testUnit->setErrorMsg("get directory");
    OSF_DirRecord* dirRecord = rootDir->get(dirName);
    testUnit->setErrorMsg("directory no exist");
    if(!dirRecord){
        return testUnit->error();
    }
    testUnit->setErrorMsg("no detect directory as a directory");
    if(!rootDir->isDir(dirRecord)){
        return testUnit->error();
    }
    //Story 2:
    // Create a file. Add to root directory. Next
    // get and test a file record object. 
    string fileName = "file name";
    testUnit->setErrorMsg("create file");
    rootDir->createFile(fileName);
    testUnit->setErrorMsg("get file");
    OSF_DirRecord* fileRecord = rootDir->get(fileName);
    testUnit->setErrorMsg("file no exist");
    if(!fileRecord){
        return testUnit->error();
    }
    testUnit->setErrorMsg("detect file as a directory");
    if(rootDir->isDir(fileRecord)){
        return testUnit->error();
    }
}

void userstoryCreateAndTestByIsFile(OSF_TestUnit* testUnit) {
    //prepare
    testUnit->setErrorMsg("create directory");
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    //Story 1:
    // Create a directory. Add to root directory. Next
    // get and test a directory record. 
    string dirName = "directory name";
    testUnit->setErrorMsg("create directory");
    rootDir->createDir(dirName);
    testUnit->setErrorMsg("get directory");
    OSF_DirRecord* dirRecord = rootDir->get(dirName);
    testUnit->setErrorMsg("directory no exist");
    if(!dirRecord){
        return testUnit->error();
    }
    testUnit->setErrorMsg("detect directory as a file");
    if(rootDir->isFile(dirRecord)){
        return testUnit->error();
    }
    //Story 2:
    // Create a file. Add to root directory. Next
    // get and test a file record object. 
    string fileName = "file name";
    testUnit->setErrorMsg("create file");
    rootDir->createFile(fileName);
    testUnit->setErrorMsg("get file");
    OSF_DirRecord* fileRecord = rootDir->get(fileName);
    testUnit->setErrorMsg("file no exist");
    if(!fileRecord){
        return testUnit->error();
    }
    testUnit->setErrorMsg("no detect file as a file");
    if(!rootDir->isFile(fileRecord)){
        return testUnit->error();
    }
}

void userstoryGetDirByPath(OSF_TestUnit* testUnit) {
    //prepare
    testUnit->setErrorMsg("create directory");
    OSF_FileSystem* fs = testUnit->getFileSystem();
    OSF_DirectoryInterface* rootDir = fs->getRootDir();
    //Story 1:
    // Create a directories and sub-directories. 
    // Next get one by path
    string d3f1Filename = "d3-f1";
    testUnit->setErrorMsg("create directory");
    //WARNING:
    // (OSF_Directory*) is for easy debugger.
    // Unfortunately the test will be work only with OSF_Directory
    OSF_Directory* d1 = (OSF_Directory*)rootDir->createDir("d1");
    OSF_Directory* d2 = (OSF_Directory*)d1->createDir("d2");
    OSF_Directory* d3 = (OSF_Directory*)d2->createDir("d3");
    OSF_FileInterface* d3f1 = (OSF_File*)d3->createFile(d3f1Filename);
    OSF_Directory* d4 = (OSF_Directory*)d3->createDir("d4");
    testUnit->setErrorMsg("get directory");
    string path = "d1/d2/d3";
    OSF_Directory* dirRecord = (OSF_Directory*)rootDir->getDirectory(path);
    testUnit->setErrorMsg("directory no exist");
    if(!dirRecord){
        return testUnit->error();
    }
    testUnit->setErrorMsg("select directory don't contain 'd3f1' file (no get by name)");
    if(dirRecord->getFile(d3f1Filename)==NULL){
        return testUnit->error();
    }
    testUnit->setErrorMsg("select directory don't contain 'd4' directory (no get by name)");
    if(dirRecord->getDirectory("d4")==NULL){
        return testUnit->error();
    }
    //delete
    delete d1, d2, d3, d3f1, d4;
}

int main(int argc, char** argv) {
    
    OSF_TestUnit testUnit;
    
    testUnit.startTests("OSF_DirectoryTest_Basic");
    
    testUnit.test("userstoryCreateAndGetFile", &userstoryCreateAndGetFileRecord);
    testUnit.test("userstoryGetFile", &userstoryGetFile);
    testUnit.test("userstoryGetDirectory", &userstoryGetDirectory);
    testUnit.test("userstoryCreateAndTestByIsDir", &userstoryCreateAndTestByIsDir);
    testUnit.test("userstoryCreateAndTestByIsFile", &userstoryCreateAndTestByIsFile);
    testUnit.test("testSearchPath", &userstoryGetDirByPath);

    testUnit.endTests();
    
    return (EXIT_SUCCESS);
}

