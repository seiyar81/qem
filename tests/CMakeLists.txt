cmake_minimum_required(VERSION 2.8)

project(qemTests)

include(../QemWrapCpp.cmake)

#---------------------------------------------------------------------------$
#                             QT - Variables
#---------------------------------------------------------------------------$
find_package(Qt4 4.8.1 COMPONENTS QtCore REQUIRED)

include(${QT_USE_FILE})

#------------------------------------------------------------------------------
#                             INCLUDE DIRECTORIES
#------------------------------------------------------------------------------

add_subdirectory( unittest-cpp )

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/unittest-cpp/
	${qem_INCLUDE_DIR}
)

SET(SOURCES
	main.cpp
)

SET(HEADERS
        fixtures.h
        watcher.h
)

SET(MOC_HEADERS
        watcher.h
)

SET(QEM_HEADERS
        fixtures.h
)

#------------------------------------------------------------------------------
#                                Qt MACROS
#------------------------------------------------------------------------------
qem_wrap_cpp(QEM_OUTFILES ${PROJECT_BINARY_DIR} ${QEM_HEADERS})
qt4_wrap_cpp(MOC_OUTFILES ${MOC_HEADERS})
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

