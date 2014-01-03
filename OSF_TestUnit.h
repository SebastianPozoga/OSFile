/* 
 * File:   OSF_TestUnit.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 4:47 PM
 */

#ifndef OSF_TESTUNIT_H
#define	OSF_TESTUNIT_H

#include "OSF_Types.h"

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

public:
    
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
        error( getErrorMsg() );
    }

    void endTest() {
        OSF_TTime time = this->timerEnd(startTestTime);
        std::cout << "%TEST_FINISHED% time=" << time << " " << currentTest << " (" << currentClass << ")" << std::endl;
    }

    void test(string name, void (*testFunction)(OSF_TestUnit*)) {
        startTest(name);
        try {
            (*testFunction)((OSF_TestUnit*)this);
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
    string filePath(string fileName){
        string base = OSF_TEST_PATH;
        return base+fileName;
    }
};


#endif	/* OSF_TESTUNIT_H */

