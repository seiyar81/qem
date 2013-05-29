cmake_minimum_required(VERSION 2.8)

project(qemTests)

include(../QemWrapCpp.cmake)

#---------------------------------------------------------------------------$
#                             QT - Variables
#---------------------------------------------------------------------------$

# We try to locate Qt5 first
find_package( Qt5Core QUIET )
if(Qt5Core_FOUND)

	message("Building Qem tests against Qt5Core")

	include_directories(${Qt5Core_INCLUDE_DIRS})
	add_definitions(${Qt5Core_DEFINITIONS})

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	set(QT_LIBRARIES ${Qt5Core_LIBRARIES})

else()
	find_package(Qt4 4.8.1 COMPONENTS QtCore REQUIRED)
	include(${QT_USE_FILE})
endif()

#------------------------------------------------------------------------------
#                             INCLUDE DIRECTORIES
#------------------------------------------------------------------------------

add_subdirectory( unittest-cpp )

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/unittest-cpp/
	${qem_INCLUDE_DIR}
)

set(SOURCES
	main.cpp
)

set(HEADERS
        fixtures.h
        watcher.h
)

set(MOC_HEADERS
        watcher.h
)

set(QEM_HEADERS
        fixtures.h
)

#------------------------------------------------------------------------------
#                                Qt MACROS
#------------------------------------------------------------------------------
get_property(qemPreproc_loc TARGET qemPreproc PROPERTY LOCATION)
get_filename_component(QEM_PREPROC_BIN ${qemPreproc_loc} NAME)

qem_wrap_cpp(QEM_OUTFILES ${QEM_PREPROC_BIN} ${PROJECT_BINARY_DIR} ${QEM_HEADERS})

if(NOT Qt5Core_FOUND)
	qt4_wrap_cpp(MOC_OUTFILES ${MOC_HEADERS})
endif()

#------------------------------------------------------------------------------
#                                LIBRARY
#------------------------------------------------------------------------------
add_executable(qemTests
	${SOURCES}
	${HEADERS}
	${MOC_OUTFILES}
	${QEM_OUTFILES}
)

target_link_libraries(qemTests
	${QT_LIBRARIES}
	qem
	UnitTest++
)
