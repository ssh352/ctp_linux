cmake_minimum_required(VERSION 2.8.4)

add_compile_options(-std=c++11)
find_library(LIBTHOSTMDUSERAPI_PATH thostmduserapi ../lib)
# find_library(LIBZMQ_PATH libzmq ../lib)
# find_library(LIBPROTOBUF_PATH glog ../lib)
# find_library(LIBPROTOBUF_PATH libprotobufd ../lib)
# find_library(LIBPROTOBUF_PATH libprotobufd ../lib)

if(NOT LIBTHOSTMDUSERAPI_PATH)
	MESSAGE(FATAL_ERROR "thostmduserapi lib not found")
ENDIF(NOT LIBTHOSTMDUSERAPI_PATH)

# if(NOT LIBZMQ_PATH)
#         MESSAGE(FATAL_ERROR "libzmq lib not found")
# ENDIF(NOT LIBZMQ_PATH)

# if(NOT LIBPROTOBUF_PATH)
#         MESSAGE(FATAL_ERROR "libprotobuf lib not found")
# ENDIF(NOT LIBPROTOBUF_PATH)

include_directories(../include)
include_directories(../include/api)
include_directories(../include/rapidjson)
include_directories(../include/zmq)
# include_directories(../include/google)


# link_directories("..\\lib")
link_directories(${PROJECT_SOURCE_DIR}/lib)
add_executable(main CtpMd.cpp MdSpi.cpp ../include/FileUtils.cpp ../include/ctp.pb.cc)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# if you want to use link_directories, you should let add_executable after link_directories
# target_link_libraries(main thostmduserapi})
target_link_libraries(main ${LIBTHOSTMDUSERAPI_PATH})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	message(STATUS "current platform:linux")
	target_link_libraries(main zmq)
	target_link_libraries(main glog)
	target_link_libraries(main protobuf)
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")
# target_link_libraries(main ${LIBZMQ_PATH})
# target_link_libraries(main ${LIBPROTOBUF_PATH})
