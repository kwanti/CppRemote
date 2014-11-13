
cmake_minimum_required(VERSION 2.8.8)
project(CppRemote CXX)

# options are None, Debug, Release, RelWithDebInfo, MinSizeRel
set(CMAKE_BUILD_TYPE Release)

# check prerequisites
if(NOT Boost_FOUND)
	set(Boost_USE_MULTITHREADED ON)		# default is ON, use -mt variant
	set(Boost_USE_STATIC_LIBS ON)		# default is OFF, use boost static library
#	set(Boost_USE_STATIC_RUNTIME ON)	# default is not set.

	find_package(Boost 1.50.0 COMPONENTS serialization thread system chrono)
	if(NOT Boost_FOUND)
		set(BOOST_ROOT "C:/boost/boost_1_57_0")						# if not set, it will let cmake to find it.
		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/x86)			# might need to set this too.
		set(Boost_NO_SYSTEM_PATHS ON)								# default is OFF, set to ON if BOOST_ROOT is set
		find_package(Boost 1.50.0 COMPONENTS serialization thread system chrono REQUIRED)
	endif()
endif()


# project configuration
set(CppRemote_ROOT ${CMAKE_SOURCE_DIR})
set(CppRemote_SRC_DIR ${CppRemote_ROOT}/src)
set(CppRemote_INCLUDE_DIR ${CppRemote_ROOT}/include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CppRemote_SOURCE_FILES_remote
	${CppRemote_SRC_DIR}/remote/exception.cpp
	${CppRemote_SRC_DIR}/remote/future_error.cpp
	${CppRemote_SRC_DIR}/remote/global.cpp
	${CppRemote_SRC_DIR}/remote/io_runner.cpp
	${CppRemote_SRC_DIR}/remote/pool_node.cpp
	${CppRemote_SRC_DIR}/remote/register_archive.cpp
	${CppRemote_SRC_DIR}/remote/server.cpp
	${CppRemote_SRC_DIR}/remote/session.cpp
)
set(CppRemote_SOURCE_FILES_remote_archive
	${CppRemote_SRC_DIR}/remote/archive/binary_iarchive.cpp
	${CppRemote_SRC_DIR}/remote/archive/text_iarchive.cpp
	${CppRemote_SRC_DIR}/remote/archive/xml_iarchive.cpp
)
set(CppRemote_SOURCE_FILES_remote_bindings	
	${CppRemote_SRC_DIR}/remote/bindings/buffer_pool.cpp
	${CppRemote_SRC_DIR}/remote/bindings/tcp_acceptor.cpp
	${CppRemote_SRC_DIR}/remote/bindings/tcp_endpoint.cpp
	${CppRemote_SRC_DIR}/remote/bindings/tcp_transport.cpp
	${CppRemote_SRC_DIR}/remote/bindings/udp_acceptor.cpp
	${CppRemote_SRC_DIR}/remote/bindings/udp_endpoint.cpp
	${CppRemote_SRC_DIR}/remote/bindings/udp_transport.cpp
	${CppRemote_SRC_DIR}/remote/bindings/local_acceptor.cpp
	${CppRemote_SRC_DIR}/remote/bindings/local_transport.cpp
	${CppRemote_SRC_DIR}/remote/bindings/named_pipe_acceptor.cpp
	${CppRemote_SRC_DIR}/remote/bindings/named_pipe_transport.cpp
)
set(CppRemote_SOURCE_FILES_remote_detail
	${CppRemote_SRC_DIR}/remote/detail/call.cpp
	${CppRemote_SRC_DIR}/remote/detail/comm_link.cpp
	${CppRemote_SRC_DIR}/remote/detail/direct_link.cpp
	${CppRemote_SRC_DIR}/remote/detail/pool_link.cpp
	${CppRemote_SRC_DIR}/remote/detail/pool_node.cpp
	${CppRemote_SRC_DIR}/remote/detail/proxy.cpp
	${CppRemote_SRC_DIR}/remote/detail/proxy_pool.cpp
	${CppRemote_SRC_DIR}/remote/detail/result.cpp
	${CppRemote_SRC_DIR}/remote/detail/server.cpp
	${CppRemote_SRC_DIR}/remote/detail/service_pool.cpp
	${CppRemote_SRC_DIR}/remote/detail/type_id.cpp
)
set(CppRemote_SOURCE_FILES_remote_serialization_system
	${CppRemote_SRC_DIR}/remote/serialization/system/error_categories.cpp
)
set(CppRemote_SOURCE_FILES
	${CppRemote_SOURCE_FILES_remote}
	${CppRemote_SOURCE_FILES_remote_archive}
	${CppRemote_SOURCE_FILES_remote_bindings}
	${CppRemote_SOURCE_FILES_remote_detail}
	${CppRemote_SOURCE_FILES_remote_serialization_system}
)

#source_group("src\\remote" FILES ${CppRemote_SOURCE_FILES_remote})
#source_group("src\\remote\\archive" FILES ${CppRemote_SOURCE_FILES_remote_archive})
#source_group("src\\remote\\bindings" FILES ${CppRemote_SOURCE_FILES_remote_bindings})
#source_group("src\\remote\\detail" FILES ${CppRemote_SOURCE_FILES_remote_detail})
#source_group("src\\remote\\serialization\\system" FILES ${CppRemote_SOURCE_FILES_remote_serialization_system})

# compilers configuration
if(WIN32)
	add_definitions(-D_WIN32_WINNT=0x0501 -D_SCL_SECURE_NO_WARNINGS)
	if(NOT MSVC)
		add_definitions(-DBOOST_THREAD_USE_LIB)
	endif()
endif()

# build configurations
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CppRemote_INCLUDE_DIR})

add_library(remote STATIC ${CppRemote_SOURCE_FILES})
