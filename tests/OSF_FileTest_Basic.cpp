/* 
 * File:   OSF_FileTest_Basic.cpp
 * Author: spozoga
 *
 * Created on Jan 4, 2014, 12:03:21 PM
 */

#include <stdlib.h>
#include <iostream>
#include "OSF_File.h"


void testRead(OSF_TestUnit* testUnit) {
}

void testWrite(OSF_TestUnit* testUnit) {
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% OSF_FileTest_Basic" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testRead (OSF_FileTest_Basic)" << std::endl;
    testRead();
    std::cout << "%TEST_FINISHED% time=0 testRead (OSF_FileTest_Basic)" << std::endl;

    std::cout << "%TEST_STARTED% testWrite (OSF_FileTest_Basic)" << std::endl;
    testWrite();
    std::cout << "%TEST_FINISHED% time=0 testWrite (OSF_FileTest_Basic)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

