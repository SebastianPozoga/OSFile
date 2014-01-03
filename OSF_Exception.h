/* 
 * File:   OSF_Exception.h
 * Author: spozoga
 *
 * Created on January 2, 2014, 9:26 PM
 */

#ifndef OSF_EXCEPTION_H
#define	OSF_EXCEPTION_H

#include <iostream>
#include <string>

using namespace std;

class OSF_Exception {
private:
    string message;
    int code;
    
    
public:
    OSF_Exception(string message, int errorCode = 1);
    
    string getMessage();
    
    int getCode();
    
    
    virtual ~OSF_Exception();

};

#endif	/* OSF_EXCEPTION_H */

