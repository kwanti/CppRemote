
cmake_minimum_required(VERSION 2.8.8)
project(Example3 CXX)

# options are None, Debug, Release, RelWithDebInfo, MinSizeRel
set(CMAKE_BUILD_TYPE Release)

# check prerequisites
if(NOT Boost_FOUND)
	set(Boost_USE_MULTITHREADED ON)		# default is ON, use -mt variant
	set(Boost_USE_STATIC_LIBS ON)		# default is OFF, use boost static library
#	set(Boost_USE_STATIC_RUNTIME ON)	# default is not set.

	find_package(Boost 1.50.0 COMPONENTS serialization thread system chrono)
	if(NOT Boost_FOUND)
		set(BOOST_ROOT "C:/Program Files/boost/boost_1_50_0")		# if not set, it will let cmake to find it.
		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/x86)			# might need to set this too.
		set(Boost_NO_SYSTEM_PATHS ON)								# default is OFF, set to ON if BOOST_ROOT is set
		find_package(Boost 1.50.0 COMPONENTS serialization thread system chrono REQUIRED)
	endif()
endif()

if(NOT Remote_FOUND)
	set(CPPREMOTE_ROOT ${PROJECT_SOURCE_DIR}/../..)
	set(Remote_INCLUDE_DIR ${CPPREMOTE_ROOT}/include)
	
	if(MSVC)
		set(Remote_LIBRARY_PATH_DEBUG ${CPPREMOTE_ROOT}/lib/Debug)
		set(Remote_LIBRARY_PATH_RELEASE ${CPPREMOTE_ROOT}/lib/Release)
	else()
		set(Remote_LIBRARY_PATH_DEBUG ${CPPREMOTE_ROOT}/lib)
		set(Remote_LIBRARY_PATH_RELEASE ${CPPREMOTE_ROOT}/lib)
	endif()
	find_library(Remote_LIBRARY_DEBUG remote ${Remote_LIBRARY_PATH_DEBUG})
	find_library(Remote_LIBRARY_RELEASE remote ${Remote_LIBRARY_PATH_RELEASE})
	if(NOT Remote_LIBRARY_DEBUGNOTFOUND)
		if(NOT Remote_LIBRARY_RELEASENOTFOUND)
			set(Remote_FOUND)
			set(Remote_LIBRARY debug ${Remote_LIBRARY_DEBUG} optimized ${Remote_LIBRARY_RELEASE})
		endif()
	endif()
endif()

# project configuration
set(Example_ROOT ${PROJECT_SOURCE_DIR})
set(Example_SRC_DIR ${Example_ROOT}/src)
set(Example_INCLUDE_DIR ${Example_ROOT}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

set(Example_SOURCE_FILES_client
	${Example_SRC_DIR}/remote_player.cpp
	${Example_SRC_DIR}/remote_ac.cpp
	${Example_SRC_DIR}/info_service.cpp
	${Example_SRC_DIR}/client.cpp
)

set(Example_SOURCE_FILES_server
	${Example_SRC_DIR}/remote_player.cpp
	${Example_SRC_DIR}/remote_ac.cpp
	${Example_SRC_DIR}/info_service.cpp
	${Example_SRC_DIR}/server.cpp
)

# compilers configuration
if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x0501 -D_SCL_SECURE_NO_WARNINGS)
	if(NOT MSVC)
		add_definitions(-DBOOST_THREAD_USE_LIB)
	endif()
endif()

# strip debug symbols
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
	set(CMAKE_EXE_LINKER_FLAGS_MinSizeRel "${CMAKE_EXE_LINKER_FLAGS_MinSizeRel} -s")
endif()

# build configurations
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Remote_INCLUDE_DIR})
include_directories(${Example_INCLUDE_DIR})

add_executable(client ${Example_SOURCE_FILES_client})
add_executable(server ${Example_SOURCE_FILES_server})

# link target.  link order of link is significant.
target_link_libraries(client ${Remote_LIBRARY})
target_link_libraries(server ${Remote_LIBRARY})

target_link_libraries(client ${Boost_LIBRARIES})
target_link_libraries(server ${Boost_LIBRARIES})

if(WIN32)
	target_link_libraries(client ws2_32 mswsock)
	target_link_libraries(server ws2_32 mswsock)
endif()
