/* 
 * File:   OSF_VHDDInterface.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 3:19 PM
 */

#ifndef OSF_VHDDINTERFACE_H
#define	OSF_VHDDINTERFACE_H

#include "OSF_Types.h"
#include "OSF_Exception.h"

class OSF_VHDDInterface {
public:

    /**
     * Read sector or sector from virtual disk to buffer
     * 
     * @param sectorNumber first sector to read
     * @param buffer Memory buffer to storage output
     * @param sectorCount number of sectors to read
     * @return 
     */
    virtual OSF_SectorInt read(OSF_SectorInt sectorNumber, char* buffer, OSF_SectorInt sectorCount){
        throw OSF_Exception("OSF_VHDDInterface No implement read", 301);
    }

    /**
     * Read sector or sectors from virtual disk to buffer
     * 
     * @param sectorNumber first sector to write
     * @param buffer Memory buffer with data
     * @param sectorCount number of sectors to write
     * @return 
     */
    virtual OSF_SectorInt write(OSF_SectorInt sectorNumber, char* buffer, OSF_SectorInt sectorCount){
        throw OSF_Exception("OSF_VHDDInterface No implement write", 301);
    }

    /**
     * Getter: Return size of one sector (in bytes)
     * @return sector size
     */
    virtual OSF_SectorSizeInt getSectorSize(){
        throw OSF_Exception("OSF_VHDDInterface No implement getSectorSize", 301);
    }

    /**
     * Getter: Return sectors quantity
     * @return a count of disk sector
     */
    virtual OSF_SectorInt getSectorCount(){
        throw OSF_Exception("OSF_VHDDInterface No implement getSectorCount", 301);
    }

    /**
     * Delete unused handles
     */
    virtual ~OSF_VHDDInterface(){
    }

};

#endif	/* OSF_VHDDINTERFACE_H */

