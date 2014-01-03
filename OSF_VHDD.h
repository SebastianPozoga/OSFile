/* 
 * File:   VHDD.h
 * Author: spozoga
 *
 * Created on December 25, 2013, 12:16 AM
 */

#ifndef VHDD_H
#define	VHDD_H

#include <stdio.h>
#include <string>

#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

using namespace std;

/*Terms*/
// sector - A sector is a division, usually 512 or 2048 bytes large, of a storage device like a hard drive or flash drive.

//Represented sector number or count
#define OSF_SectorInt unsigned int

//Represented sector size
#define OSF_SectorSizeInt unsigned int

/**
 * Class represented a virtual device. The VHDD (Virtual Hard Disk Driver) storage all informations;
 * Provide API to persist context.
 * 
 * @param path Path to file contains all data; 
 * @param sectorSize A byte per sector
 * @param sectorCount A sector count
 */
class OSF_VHDD {
private:
    //handle to disk file
    FILE* file;
    //fstream fs;

    //path to virtual disk file
    std::string path;

    //Size of one sector (in bytes)
    OSF_SectorSizeInt sectorSize;

    //Sectors quantity
    OSF_SectorInt sectorCount;

public:

    /**
     * Create object represented new virtual disk
     * @param path
     * @param sectorSize
     * @param sectorCount
     */
    OSF_VHDD(string path, OSF_SectorSizeInt sectorSize, OSF_SectorInt sectorCount, bool cleanFile=false);

    /**
     * Read sector or sector from virtual disk to buffer
     * 
     * @param sectorNumber first sector to read
     * @param buffer Memory buffer to storage output
     * @param sectorCount number of sectors to read
     * @return 
     */
    OSF_SectorInt read(OSF_SectorInt sectorNumber, char* buffer, OSF_SectorInt sectorCount);

    /**
     * Read sector or sectors from virtual disk to buffer
     * 
     * @param sectorNumber first sector to write
     * @param buffer Memory buffer with data
     * @param sectorCount number of sectors to write
     * @return 
     */
    OSF_SectorInt write(OSF_SectorInt sectorNumber, char* buffer, OSF_SectorInt sectorCount);

    /**
     * Getter: Return size of one sector (in bytes)
     * @return sector size
     */
    OSF_SectorSizeInt getSectorSize();

    /**
     * Getter: Return sectors quantity
     * @return a count of disk sector
     */
    OSF_SectorInt getSectorCount();

    /**
     * Delete unused handles
     */
    virtual ~OSF_VHDD();

};

#endif	/* VHDD_H */

