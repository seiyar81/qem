cmake_minimum_required(VERSION 2.8)

project(qemPreproc)

#---------------------------------------------------------------------------$
#                             QT - Variables
#---------------------------------------------------------------------------$
# We try to locate Qt5 first
find_package( Qt5Core QUIET )
if(Qt5Core_FOUND)

	message("Building qemPreProc against Qt5Core")

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

install( TARGETS qemPreproc
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
