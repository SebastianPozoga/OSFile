/* 
 * File:   OSF_TestUnit.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 4:47 PM
 */

#ifndef OSF_TESTUNIT_H
#define	OSF_TESTUNIT_H

#include "OSF_Types.h"
#include "OSF_VHDD.h"
#include "OSF_FileSystem.h"

#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <ctime>
#include <iostream>
#include <time.h>

//create dir (LINUX ONLY)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define OSF_TTime long double
#define OSF_TEST_PATH "./OSF_test_data/"


using namespace std;

class OSF_TestUnit {
private:
    string currentClass;
    string currentTest;
    clock_t startClassTime;
    clock_t startTestTime;
    string errorMsg;

    //Most use library
    OSF_VHDD* vHDD;
    OSF_FileSystem* fileSystem;
    OSF_FileSystemHeader* fsHeader;

public:

    OSF_TestUnit() {
        vHDD = NULL;
        fileSystem = NULL;
        fsHeader = NULL;
    }

    void startTests(string currentClass) {
        this->mkdir(OSF_TEST_PATH);
        this->currentClass = currentClass;
        this->startClassTime = this->timerStart();

        std::cout << "%SUITE_STARTING% " << currentClass << std::endl;
        std::cout << "%SUITE_STARTED%" << std::endl;
    }

    void startTest(string currentTest) {
        this->currentTest = currentTest;
        this->startTestTime = this->timerStart();
        this->errorMsg = "some exception";
        std::cout << "%TEST_STARTED% " << currentTest << " (" << currentClass << ")" << std::endl;
    }

    void error(string msg) {
        OSF_TTime time = this->timerEnd(startTestTime);
        std::cout << "%TEST_FAILED% time=" << time << " testname=" << currentTest << " (" << currentClass << ") message=" << msg << std::endl;
    }

    void error() {
        error(getErrorMsg());
    }

    void endTest() {
        OSF_TTime time = this->timerEnd(startTestTime);
        std::cout << "%TEST_FINISHED% time=" << time << " " << currentTest << " (" << currentClass << ")" << std::endl;
        //clean
        if (fileSystem != NULL) {
            delete fileSystem;
            fileSystem = NULL;
        }
        if (fsHeader != NULL) {
            delete fsHeader;
            fsHeader = NULL;
        }
        if (vHDD != NULL) {
            delete vHDD;
            vHDD = NULL;
        }
    }

    void test(string name, void (*testFunction)(OSF_TestUnit*)) {
        startTest(name);
        try {
            (*testFunction)((OSF_TestUnit*)this);
        } catch (string s) {
            this->error("throw String: " + errorMsg + "(" + s + ")");
        } catch (OSF_Exception e) {
            this->error("throw OSF_Exception: " + errorMsg + "(" + e.getMessage() + ")");
        } catch (...) {
            this->error("throw: " + errorMsg);
        }
        endTest();
    }

    void endTests() {
        OSF_TTime time = this->timerEnd(startClassTime);
        std::cout << "%SUITE_FINISHED% time=" << time << std::endl;
    }

    string getErrorMsg() const {
        return errorMsg;
    }

    void setErrorMsg(string errorMsg) {
        this->errorMsg = errorMsg;
    }

protected:
    /**
     * TIMER
     * {THIS VERSION WORK ONLY ON LINUX}
     * {clock() - gives cpu time on Linux and unfortunately return WALL TIME on Windows}
     * {PS. My favorite is LINUX ;-) }
     */

    // call this function to start a nanosecond-resolution timer

    clock_t timerStart() {
        return clock();
    }

    // call this function to end a timer, returning nanoseconds elapsed as a long

    OSF_TTime timerEnd(clock_t startTime) {
        return ((OSF_TTime) clock() - startTime) / CLOCKS_PER_SEC;
    }

public:
    /**
     * Helpers
     */

    /**
     * Create directory
     * {LINUX ONLY}
     */
    void mkdir(string path) {
        struct stat st = {0};
        if (stat(path.c_str(), &st) == -1) {
            ::mkdir(path.c_str(), 0777);
        }
    }

    /**
     * Create path for test file
     * 
     * @param fileName global UNIQUE file name
     * @return path to file
     */
    string filePath(string fileName) {
        string base = OSF_TEST_PATH;
        return base + fileName;
    }

    OSF_VHDD* getVHDD() {
        if (vHDD == NULL) {
            string name = currentClass + "-" + currentTest;
            vHDD = new OSF_VHDD(filePath(name), 128, 30, true);
        }
        return vHDD;
    }

    OSF_FileSystemHeader* getFileSystemHader() {
        if (fsHeader == NULL) {
            fsHeader = new OSF_FileSystemHeader;
            fsHeader->allocPointer = 0;
            fsHeader->clusterCount = 0;
            strcpy(fsHeader->diskName, "disk name");
            strcpy(fsHeader->systemName, "system name");
        }
        return fsHeader;
    }

    OSF_FileSystem* getFileSystem() {
        if (fileSystem == NULL) {
            OSF_VHDD* vHDD = getVHDD();
            OSF_FileSystemHeader* fsHeader = getFileSystemHader();
            fileSystem = new OSF_FileSystem(vHDD, fsHeader, 2);
        }
        return fileSystem;
    }

    char* createBuffer(long int size, long int repeatLong = 30) {
        char* buffer = new char[size];
        writeBufferTestData(buffer, size, repeatLong);
        return buffer;
    }

    char* writeBufferTestData(char* buffer, long int size, long int repeatLong = 30) {
        for (long int i = 0; i < size; i++) {
            buffer[i] = (i % repeatLong) + 1;
        }
        return buffer;
    }

    void cleanBuffer(char* buffer, long int size) {
        memset(buffer, '?', size);
    }

    bool testBuffer(char* buffer, long int size, long int repeatLong = 30) {
        for (long int i = 0; i < size; i++) {
            if (buffer[i] != (i % repeatLong) + 1) {
                return false;
            }
        }
        return true;
    }
};


#endif	/* OSF_TESTUNIT_H */

