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
    $ENV{SFML_ROOT}
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
				 
find_program(QEM_PREPROC_BIN
             NAMES qemPreProc
			 PATH_SUFFIXES bin
             PATHS ${QEM_ROOT})
			 
set(QEM_CMAKE_FILE ${QEM_ROOT}/QemWrapCpp.cmake)
