#CMakeのバージョンの設定 
cmake_minimum_required(VERSION 3.1.0)

#プロジェクト名
project(rs-capture)


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

find_package(OpenCV REQUIRED)

set(DEPENDENCIES realsense2 ${OpenCV_LIBS} )

add_executable(rs-capture ../src/rs-capture.cpp)
target_link_libraries(rs-capture ${DEPENDENCIES})