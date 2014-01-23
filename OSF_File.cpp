
#include "OSF_File.h"
#include "OSF_Exception.h"

/*Create object represented exist file*/
OSF_File::OSF_File(OSF_FileSystemInterface* fileSystem, OSF_ClusterInt firstCluster) : OSF_DiskList(fileSystem, firstCluster) {
}

/*Create new file*/
OSF_File::OSF_File(OSF_FileSystemInterface* fileSystem, OSF_FileHeder* header, OSF_ClusterInt firstCluster) : OSF_DiskList(fileSystem, header, firstCluster) {
}

void OSF_File::initNew() {
    this->getHeaderPtr()->fileSize = 0;
}

/*void OSF_File::create() {
    OSF_FileHeder header;
    header.fileSize = 0;

}*/


OSF_ClusterInt OSF_File::read(OSF_Buffer ptr, OSF_ClusterInt firstToRead, OSF_ClusterInt count) {
    ////skip all before
    OSF_FileRecord record;
    first(&record);
    for (int i = 0; i < firstToRead && current(&record); i++, next(&record));

    //read
    OSF_ClusterInt clusterSize = getFileSystem()->getClusterSize();
    OSF_ClusterInt readedCluster = 0;
    char* bufferPtr = (char*) ptr;
    for (int i = 0; i < count && current(&record); i++, next(&record)) {
        try {
            this->getFileSystem()->read(record.cluster, bufferPtr, 1);
        } catch (OSF_Exception ex) {
            if (ex.getCode() != 420) {
                throw ex;
            }
        }
        bufferPtr = &bufferPtr[clusterSize];
        readedCluster++;
    }
    return readedCluster;
}

OSF_ClusterInt OSF_File::write(OSF_Buffer ptr, OSF_ClusterInt firstToWrite, OSF_ClusterInt count) {
    int c = 0;
    ////skip
    OSF_FileRecord record;

    first(&record);
    for (; c < firstToWrite && current(&record); c++, next(&record));
    for (; c < firstToWrite; c++, next(&record)) {
        record.cluster = getFileSystem()->allocCluster();
        push(&record);
    }

    //read
    OSF_ClusterInt clusterSize = getFileSystem()->getClusterSize();
    OSF_ClusterInt readedCluster = 0;
    char* bufferPtr = (char*) ptr;
    for (int i = 0; i < count; i++, next(&record)) {
        if (current(&record) == NULL) {
            OSF_FileRecord newRecord;
            newRecord.cluster = getFileSystem()->allocCluster();
            if (!push(&newRecord)) {
                throw OSF_Exception("OSF_File::write Push file cluster error");
            }
            current(&record);
            if (record.cluster != newRecord.cluster) {
                throw OSF_Exception("OSF_File::write Iteration error");
            }
        }
        //OSF_ClusterInt writeCluster = record.cluster;
        this->getFileSystem()->write(record.cluster, bufferPtr, 1);
        bufferPtr = &bufferPtr[clusterSize];
        readedCluster++;
    }

    return readedCluster;
}

OSF_FileHeder* OSF_File::readFileHeder(OSF_FileHeder* header) {
    return this->readHeader(header);
}

OSF_FileHeder* OSF_File::writeFileHeder(OSF_FileHeder* header) {
    return this->writeHeader(header);
}
