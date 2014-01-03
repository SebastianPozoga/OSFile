/* 
 * File:   OSF_DirTestBasic.cpp
 * Author: spozoga
 *
 * Created on Jan 2, 2014, 10:54:58 PM
 */

#include <stdlib.h>
#include <OSF_Dir.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "OSF_DirTestBasic test 1" << std::endl;
}

void test2() {
    std::cout << "OSF_DirTestBasic test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (OSF_DirTestBasic) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% OSF_DirTestBasic" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (OSF_DirTestBasic)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (OSF_DirTestBasic)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (OSF_DirTestBasic)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (OSF_DirTestBasic)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

