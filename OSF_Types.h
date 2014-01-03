/* 
 * File:   OSF_Types.h
 * Author: spozoga
 *
 * Created on January 3, 2014, 2:00 PM
 */

#ifndef OSF_TYPES_H
#define	OSF_TYPES_H

/**
 * Virtual HDD 
 */
// Terms:
// sector - A sector is a division, usually 512 or 2048 bytes large, of a storage device like a hard drive or flash drive.

//Represented sector number or count
#define OSF_SectorInt unsigned int

//Represented sector size
#define OSF_SectorSizeInt unsigned int


/**
 * File System
 */

//Represented cluster number or count 
#define OSF_ClusterInt unsigned int

//Offset in a claster
#define OSF_ClusterOffset unsigned int


/**
 * File
 */

#define OSF_FilePointer unsigned int
#define OSF_Size unsigned int
#define OSF_Buffer void*

/**
 * Directory
 */

/*Terms*/
//*A "directory resource" mean file or sub-directory
//*A "sub-directory" mean a directory which parent is current directory

//Owners
#define OSF_OWNER unsigned int
#define OSF_GID   unsigned int

//File type flags
#define OSF_RESOURCE_TYPE_MASK 0x200
#define OSF_RESOURCE_TYPE_FILE 0x200
#define OSF_RESOURCE_TYPE_DIR  0

//File permission mask
#define OSF_RESOURCE_PERMISSION_MASK  0x1ff

//User permission: r(Read), w(Write), x(eXecute)
#define OSF_UR  1<<0
#define OSF_UW  1<<1
#define OSF_UX  1<<2

//Group permission: r(Read), w(Write), x(eXecute)
#define OSF_GR  1<<3
#define OSF_GW  1<<4
#define OSF_GX  1<<5

//Other permission: r(Read), w(Write), x(eXecute)
#define OSF_OR  1<<6
#define OSF_OW  1<<7
#define OSF_OX  1<<8

#endif	/* OSF_TYPES_H */

