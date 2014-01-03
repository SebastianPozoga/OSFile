/* 
 * File:   Construct.cpp
 * Author: spozoga
 *
 * Created on Dec 27, 2013, 12:34:50 PM
 */

#include <stdlib.h>
#include <iostream>
#include "OSF_VHDD.h"

/*
 * Simple C++ Test Suite
 */

void testOSF_VHDD() {
    string path;
    unsigned int sectorSize;
    unsigned int sectorCount;
    OSF_VHDD oSF_VHDD(path, sectorSize, sectorCount);
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=testOSF_VHDD (Construct) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% Construct" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testOSF_VHDD (Construct)" << std::endl;
    //testOSF_VHDD();
    std::cout << "%TEST_FINISHED% time=0 testOSF_VHDD (Construct)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (11);
}

