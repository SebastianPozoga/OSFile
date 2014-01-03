/* 
 * File:   OSF_Exception.cpp
 * Author: spozoga
 * 
 * Created on January 2, 2014, 9:26 PM
 */

#include "OSF_Exception.h"

#include <string>

using namespace std;

OSF_Exception::OSF_Exception(string message, int errorCode) : message(message), code(errorCode){
    
}

string OSF_Exception::getMessage(){
    return message;
}

int OSF_Exception::getCode(){
    return code;
}

OSF_Exception::~OSF_Exception() {
}

