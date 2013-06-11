#
# This script locates the Qem library
#

# find the Qem include directory
find_path(QEM_INCLUDE_DIR Qem/Qem
          PATH_SUFFIXES include
          PATHS
          ${QEM_ROOT}
          ~/Library/Frameworks
          /Library/Frameworks
          /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/)
		  
set(FIND_QEM_LIB_PATH
    ${QEM_ROOT}
	$ENV{QEM_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)
	
find_library(QEM_LIBRARY
                 NAMES qem
                 PATH_SUFFIXES lib64 lib
                 PATHS ${FIND_QEM_LIB_PATH})

set(FIND_QEM_BIN_PATH
	${QEM_ROOT}
	$ENV{QEM_ROOT}
	~/Library/Frameworks
	/Library/Frameworks
	/usr
	/usr/local
	/
	/sw
	/opt/local
	/opt/csw
	/opt)

find_program(QEM_PREPROC_BIN
			 NAMES qemPreproc
			 PATH_SUFFIXES bin
			 PATHS ${FIND_QEM_BIN_PATH})
			 
set(FIND_QEM_FILE_PATH
	${QEM_ROOT}
	$ENV{QEM_ROOT}
	${CMAKE_MODULE_PATH}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/cmake
	/usr
	/sw
	/opt/local
	/opt/csw
	/opt)

find_program(QEM_CMAKE_FILE
			 NAMES QemWrapCpp.cmake
			 PATHS ${FIND_QEM_FILE_PATH})
