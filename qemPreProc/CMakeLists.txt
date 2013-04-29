cmake_minimum_required(VERSION 2.8)

project(qemPreproc)

#---------------------------------------------------------------------------$
#                             QT - Variables
#---------------------------------------------------------------------------$
find_package(Qt4 4.8.1 COMPONENTS QtCore REQUIRED)

include(${QT_USE_FILE})

#------------------------------------------------------------------------------
#                             INCLUDE DIRECTORIES
#------------------------------------------------------------------------------

include_directories(
	${PROJECT_SOURCE_DIR}
	${QT_INCLUDES}
)

SET(SOURCES
	main.cpp
)

SET(HEADERS
	utils.h
)

#------------------------------------------------------------------------------
#                                LIBRARY
#------------------------------------------------------------------------------
add_executable(qemPreproc
	${SOURCES}
	${HEADERS}
	${MOC_OUTFILES}
)

target_link_libraries(qemPreproc
	${QT_LIBRARIES}
)

