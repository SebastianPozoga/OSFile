#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include OSF-Makefile.mk

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/OSF_Directory.o \
	${OBJECTDIR}/OSF_Exception.o \
	${OBJECTDIR}/OSF_File.o \
	${OBJECTDIR}/OSF_FileSystem.o \
	${OBJECTDIR}/OSF_VHDD.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f5 \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/osf

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/osf: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/osf ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/OSF_Directory.o: OSF_Directory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_Directory.o OSF_Directory.cpp

${OBJECTDIR}/OSF_Exception.o: OSF_Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_Exception.o OSF_Exception.cpp

${OBJECTDIR}/OSF_File.o: OSF_File.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_File.o OSF_File.cpp

${OBJECTDIR}/OSF_FileSystem.o: OSF_FileSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_FileSystem.o OSF_FileSystem.cpp

${OBJECTDIR}/OSF_VHDD.o: OSF_VHDD.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_VHDD.o OSF_VHDD.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/OSF_DirectoryTest_Basic.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/OSF_DiskListTest_Basic.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/OSF_FileSystemTest_Basic.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f5: ${TESTDIR}/tests/OSF_FileTest_Basic.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f5 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/OSF_VHDD_Basic.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/OSF_DirectoryTest_Basic.o: tests/OSF_DirectoryTest_Basic.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/OSF_DirectoryTest_Basic.o tests/OSF_DirectoryTest_Basic.cpp


${TESTDIR}/tests/OSF_DiskListTest_Basic.o: tests/OSF_DiskListTest_Basic.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/OSF_DiskListTest_Basic.o tests/OSF_DiskListTest_Basic.cpp


${TESTDIR}/tests/OSF_FileSystemTest_Basic.o: tests/OSF_FileSystemTest_Basic.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/OSF_FileSystemTest_Basic.o tests/OSF_FileSystemTest_Basic.cpp


${TESTDIR}/tests/OSF_FileTest_Basic.o: tests/OSF_FileTest_Basic.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/OSF_FileTest_Basic.o tests/OSF_FileTest_Basic.cpp


${TESTDIR}/tests/OSF_VHDD_Basic.o: tests/OSF_VHDD_Basic.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I. -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/OSF_VHDD_Basic.o tests/OSF_VHDD_Basic.cpp


${OBJECTDIR}/OSF_Directory_nomain.o: ${OBJECTDIR}/OSF_Directory.o OSF_Directory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/OSF_Directory.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_Directory_nomain.o OSF_Directory.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/OSF_Directory.o ${OBJECTDIR}/OSF_Directory_nomain.o;\
	fi

${OBJECTDIR}/OSF_Exception_nomain.o: ${OBJECTDIR}/OSF_Exception.o OSF_Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/OSF_Exception.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_Exception_nomain.o OSF_Exception.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/OSF_Exception.o ${OBJECTDIR}/OSF_Exception_nomain.o;\
	fi

${OBJECTDIR}/OSF_File_nomain.o: ${OBJECTDIR}/OSF_File.o OSF_File.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/OSF_File.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_File_nomain.o OSF_File.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/OSF_File.o ${OBJECTDIR}/OSF_File_nomain.o;\
	fi

${OBJECTDIR}/OSF_FileSystem_nomain.o: ${OBJECTDIR}/OSF_FileSystem.o OSF_FileSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/OSF_FileSystem.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_FileSystem_nomain.o OSF_FileSystem.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/OSF_FileSystem.o ${OBJECTDIR}/OSF_FileSystem_nomain.o;\
	fi

${OBJECTDIR}/OSF_VHDD_nomain.o: ${OBJECTDIR}/OSF_VHDD.o OSF_VHDD.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/OSF_VHDD.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/OSF_VHDD_nomain.o OSF_VHDD.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/OSF_VHDD.o ${OBJECTDIR}/OSF_VHDD_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f5 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/osf

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
