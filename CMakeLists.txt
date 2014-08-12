#
# Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
# Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
#
# Funding for this software development was provided in part by the NSF (IIS-1251069) and NIH (1R01EB018205-01).
# Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.
#
# The content is solely the responsibility of the authors and does not necessarily represent the official views of the National Institutes of Health.
#
# Code author(s):
# 		Dan Orban (dtorban)
#
 
cmake_minimum_required (VERSION 2.8.2) 
project (VISCDF)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/../install" CACHE PATH "default install path" FORCE )
endif()

#This way will auto detect all source files in the directory,
#while this will work, the preferred technique is to list out
#sourcefiles individually.  This will allow cmake to automatically
#rerun if a source file is added, which it will not do if you don't
#list the explicit source files.
#aux_source_directory(${CMAKE_SOURCE_DIR} SOURCEFILES})
#This is how you specific source file individually
set (SOURCEFILES 
		core/source/NcAttributeCollection.cpp
		core/source/NcFileReader.cpp
		core/source/NcGroup.cpp
		core/source/NcVariable.cpp
		core/source/AttributeCollection.cpp
		core/source/Dimension.cpp
		core/source/Variable.cpp
		core/source/VcContainerBase.cpp
		core/source/VcGroup.cpp  
		VCDF_FEA/source/test.cpp
		VCDF_FEA/source/FEASimulation.cpp
		VCDF_FEA/source/FEAPart.cpp
		VCDF_FEA/source/FEAFrame.cpp)
set (HEADERFILES 
		core/include/memory/CountedPtr.h
		core/include/netcdf/NcAttributeCollection.h
		core/include/netcdf/NcDimension.h
		core/include/netcdf/NcFileReader.h
		core/include/netcdf/NcGroup.h
		core/include/netcdf/NcVariable.h
		core/include/netcdf/visnetcdf.h
		core/include/AttributeCollection.h
		core/include/Dimension.h
		core/include/Variable.h
		core/include/VcContainerBase.h
		core/include/VcGroup.h
		VCDF_FEA/include/FEASimulation.h
		VCDF_FEA/include/FEAPart.h
		VCDF_FEA/include/FEAFrame.h)
source_group("Header Files" FILES ${HEADERFILES})

############################################################
#Start of library detection
#Any libraries outside of your source should have a corresponding
#find module in the $G/src/cmake/Modules directory and the 
#find_package method should be used here.
############################################################


############################################################
#End external library searching
############################################################

set (CMAKE_VERBOSE_MAKEFILE TRUE)

# Used for compiling in subdirectories of the project
include_directories (
  .
  ..
  ${VISCDF_SOURCE_DIR}/core/include
  ${VISCDF_SOURCE_DIR}/VCDF_FEA/include
)

# If you want to add any link directories you can add them here
link_directories (
  ${VISCDF_BINARY_DIR}
)

############################################################
#OS Specific flags should go here
############################################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  #set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -stdlib=libc++")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Linux specific code
 
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # Windows specific code

endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

############################################################
# Add POSTFIX info
############################################################

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
set(CMAKE_MINSIZEREL_POSTFIX "s")

#set the build postfix extension according to the current configuration
if (CMAKE_BUILD_TYPE MATCHES "Release")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
  set(CMAKE_BUILD_POSTFIX "")
endif()

############################################################
#Finally add all the targets here
############################################################

#First an example of executable target
add_executable(testfea ${SOURCEFILES})
#All of libs should be added here
#If its a debug-only lib specify it as "debug <libtoadd>"
#If its a opt-only lib specify it as "optimized <libtoadd>"
#<libtoadd> should either be a full path to the lib, or the OS
#indepdent way it would appear in the command line (i.e. -l<libtoadd>)
#If the second is way is used you need to sure the directory the lib is
#in will be found
target_link_libraries (testfea ${LIBS_DEBUG} ${LIBS_OPT} ${LIBS_ALL} netcdf)

#Second an example of a library target
#It should default to making a static library
add_library(viscdf ${SOURCEFILES} ${SOURCEFILES})

#You can also add subprojects that are in subdirectories
#These subdirectories must contain their own CMakeLists.txt
#add_subdirectory (mysubproject)

install(DIRECTORY core/include/ DESTINATION include)
install(DIRECTORY VCDF_FEA/include/ DESTINATION include)
install( TARGETS viscdf testfea
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         RUNTIME DESTINATION bin)
