cmake_minimum_required(VERSION 2.8)

project(qem CXX)

set(qem_LIB_MAJOR_VERSION "0")
set(qem_LIB_MINOR_VERSION "1")
set(qem_LIB_PATCH_VERSION "0")

set(CMAKE_CXX_FLAGS "-Wall")

include(QemWrapCpp.cmake)

#---------------------------------------------------------------------------$
#                             QT - Variables
#---------------------------------------------------------------------------$

# We try to locate Qt5 first
find_package( Qt5Core QUIET )
if(Qt5Core_FOUND)

	message("Building Qem against Qt5Core")

	include_directories(${Qt5Core_INCLUDE_DIRS})
	add_definitions(${Qt5Core_DEFINITIONS})
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	set(QT_LIBRARIES ${Qt5Core_LIBRARIES})

else()

	find_package(Qt4 4.8.1 COMPONENTS QtCore REQUIRED)
	include(${QT_USE_FILE})

endif()

if(NOT QEM_BUILD_TYPE)
	set(QEM_BUILD_TYPE STATIC)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
	add_definitions(-DQEM_DEBUG=1)
else()
	add_definitions(-DQEM_DEBUG=0)
endif()

if(APPLE)
	option(OSX_FRAMEWORK "Build a Mac OS X Framework")
	set(FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Library/Frameworks"
		CACHE PATH "Where to place qem.framework if OSX_FRAMEWORK is selected")
endif()

#------------------------------------------------------------------------------
#                             INCLUDE DIRECTORIES
#------------------------------------------------------------------------------

set(qem_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/include" CACHE STRING "QtEventableModels include directory")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE STRING "Directory where the library will be installed")
set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/cmake/${CMAKE_PROJECT_NAME}" CACHE PATH "Directory where to install QemWrapCpp.cmake")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include/" CACHE PATH "Directory where to install qem includes")

include_directories(
	${PROJECT_SOURCE_DIR}/lib
	${PROJECT_SOURCE_DIR}/lib/include
)

set(SOURCES
	lib/src/qem.cpp
	lib/src/qem_modelfield.cpp
	lib/src/qem_agency.cpp
	lib/src/qem_assert.cpp
)

set(HEADERS
	lib/include/Qem/qem.h
	lib/include/Qem/qem_agency.h
	lib/include/Qem/qem_modelfield.h
	lib/include/Qem/qem_bitfield.h
	lib/include/Qem/qem_assert.h
	lib/include/Qem/pp.h
)

set(PUBLIC_HEADERS
	${HEADERS}
	lib/include/Qem/Qem
)

#------------------------------------------------------------------------------
#                                Qt MACROS
#------------------------------------------------------------------------------

add_subdirectory( qemPreProc )

if(NOT QEMPreProcBin)
		get_property(QEMPreProcBin TARGET qemPreproc PROPERTY LOCATION)
endif()

#------------------------------------------------------------------------------
#                                LIBRARY
#------------------------------------------------------------------------------
add_library(qem ${QEM_BUILD_TYPE}
	${SOURCES}
	${HEADERS}
	${MOC_OUTFILES}
)

add_dependencies(qem qemPreProc)

set_target_properties(qem PROPERTIES
	VERSION ${qem_LIB_MAJOR_VERSION}.${qem_LIB_MINOR_VERSION}.${qem_LIB_PATCH_VERSION}
	SOVERSION ${qem_LIB_MAJOR_VERSION}
)

set_target_properties(qem PROPERTIES
	PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

if(APPLE)
	set_target_properties(qem PROPERTIES
		FRAMEWORK ${OSX_FRAMEWORK}
	)
endif()

target_link_libraries(qem
	${QT_LIBRARIES}
)

install( TARGETS qem
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
	PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}/Qem
)

install(FILES QemWrapCpp.cmake
				DESTINATION "${CMAKECONFIG_INSTALL_DIR}")

#------------------------------------------------------------------------------
#                                TESTS
#------------------------------------------------------------------------------

if(WITH_TESTS)
	add_subdirectory( tests )
endif()

