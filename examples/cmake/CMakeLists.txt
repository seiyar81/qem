cmake_minimum_required(VERSION 2.8)

project(qemCmakeExample)

# Compiler-specific flags and definitions
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
endif()

#-----------------------------------------------------------------------
#                                Qt 
#-----------------------------------------------------------------------

# We try to locate Qt5 first
find_package( Qt5Core QUIET )
if(Qt5Core_FOUND)
	message("Building example against Qt5Core")

	include_directories(${Qt5Core_INCLUDE_DIRS})
	add_definitions(${Qt5Core_DEFINITIONS})

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(CMAKE_AUTOMOC ON)
	set(QT_LIBRARIES ${Qt5Core_LIBRARIES})
else()
	find_package(Qt4 4.8.1 COMPONENTS QtCore REQUIRED)
	include(${QT_USE_FILE})
endif()


#---------------------------------------------------------------------
#                             Qem
#---------------------------------------------------------------------

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../;${CMAKE_MODULE_PATH}")

find_package( Qem )
include(${QEM_CMAKE_FILE})

#---------------------------------------------------------------------
#                             Sources
#---------------------------------------------------------------------

set(CMAKE_EXAMPLE_SOURCES
	main.cpp
)

set(CMAKE_EXAMPLE_HEADERS
	watchers.h
	models.h
)

set(CMAKE_EXAMPLE_MOC_HEADERS
    watchers.h
)

set(CMAKE_EXAMPLE_QEM_HEADERS
    models.h
)

#------------------------------------------------------------------------------
#                                Qt MACROS
#------------------------------------------------------------------------------
qem_wrap_cpp(CMAKE_EXAMPLE_QEM_OUTFILES ${QEM_PREPROC_BIN} ${PROJECT_BINARY_DIR} ${CMAKE_EXAMPLE_QEM_HEADERS})

if(NOT Qt5Core_FOUND)
	qt4_wrap_cpp(CMAKE_EXAMPLE_MOC_OUTFILES ${CMAKE_EXAMPLE_MOC_HEADERS})
endif()

#---------------------------------------------------------------------
#                                Executable
#---------------------------------------------------------------------

add_executable(qemCmakeExample
	${CMAKE_EXAMPLE_SOURCES}
	${CMAKE_EXAMPLE_HEADERS}
	${CMAKE_EXAMPLE_MOC_OUTFILES}
	${CMAKE_EXAMPLE_QEM_OUTFILES}
)

target_link_libraries(qemCmakeExample
	${QT_LIBRARIES}
	${QEM_LIBRARY}
)


