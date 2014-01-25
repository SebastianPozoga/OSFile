
#include "OSF_VHDD.h"

#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

using namespace std;

OSF_VHDD::OSF_VHDD(string path, OSF_SectorSizeInt sectorSize, OSF_SectorInt sectorCount, bool cleanFile) : path(path), sectorSize(sectorSize), sectorCount(sectorCount) {
    if (cleanFile) {
        file = fopen(path.c_str(), "wb");
        if (file == NULL) {
            fputs("FVHDD: File create error\n", stderr);
            throw "FVHDD: File create error\n";
        }
        fclose(file);
    }

    file = fopen(path.c_str(), "r+b");
    if (file == NULL) {
        //if file is not exist, create new 
        OSF_VHDD(path, sectorSize, sectorCount, true);
    }
}

OSF_SectorInt OSF_VHDD::read(OSF_SectorInt sectorNumber, char* buffer, OSF_SectorInt sectorCount) {
    if(sectorNumber+sectorCount-1>getSectorCount()){
        throw OSF_Exception("Read access to no exist sector", 420);
    }
    OSF_SectorSizeInt sSize = sectorCount * sectorSize;
    fseek(file, sectorNumber*sectorSize, SEEK_SET);
    int r = fread(buffer, 1, sSize, file);
    //fclose(file);
    return r;
}

OSF_SectorInt OSF_VHDD::write(OSF_SectorInt sectorNumber, char* buffer, OSF_SectorInt sectorCount) {
    if(sectorNumber+sectorCount-1>getSectorCount()){
        throw OSF_Exception("Write access to no exist sector", 420);
    }
    OSF_SectorSizeInt sSize = sectorCount * sectorSize;
    fseek(file, sectorNumber*sectorSize, SEEK_SET);
    int r = fwrite(buffer, 1, sSize, file);
    fflush(file);
    //fclose(file);
    return r;
}

OSF_SectorSizeInt OSF_VHDD::getSectorSize() {
    return sectorSize;
}

OSF_SectorInt OSF_VHDD::getSectorCount() {
    return sectorCount;
}

OSF_VHDD::~OSF_VHDD() {
    fclose(file);
    //fs.close();
}