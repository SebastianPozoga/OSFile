
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
    for (int i = 0; i < firstToRead && next(&record); i++);

    //read
    OSF_ClusterInt clusterSize = getFileSystem()->getClusterSize();
    OSF_ClusterInt readedCluster = 0;
    char* bufferPtr = (char*) ptr;
    for (int i = 0; i < count && next(&record); i++) {
        this->getFileSystem()->read(record.cluster, bufferPtr, 1);
        bufferPtr = &bufferPtr[clusterSize];
        readedCluster++;
    }

    return readedCluster;
}

OSF_ClusterInt OSF_File::write(OSF_Buffer ptr, OSF_ClusterInt firstToWrite, OSF_ClusterInt count) {
    int c = 0;
    ////skip
    OSF_FileRecord record;

    for (first(&record); c < firstToWrite && next(&record); c++);
    for (; c < firstToWrite && next(&record); c++) {
        record.cluster = getFileSystem()->allocCluster();
        push(&record);
    }

    //read
    OSF_ClusterInt clusterSize = getFileSystem()->getClusterSize();
    OSF_ClusterInt readedCluster = 0;
    char* bufferPtr = (char*) ptr;
    for (int i = 0; i < count && next(&record); i++) {
        OSF_ClusterInt writeCluster;
        if (current(&record) == NULL) {
            OSF_FileRecord newRecord;
            newRecord.cluster = getFileSystem()->allocCluster();
            if (push(&newRecord)) {
                throw OSF_Exception("OSF_File::write Push file cluster error");
            }
            next(&record);
            if (record.cluster != newRecord.cluster) {
                throw OSF_Exception("OSF_File::write Iteration error");
            }
        }
        writeCluster = record.cluster;
        this->getFileSystem()->read(record.cluster, bufferPtr, 1);
        bufferPtr = &bufferPtr[clusterSize];
        readedCluster++;
    }

    return readedCluster;
}
