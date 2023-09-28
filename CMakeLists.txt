# setup
cmake_minimum_required(VERSION 3.11)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# project info
project(NextServer
        VERSION 2023.09
        DESCRIPTION "A C++ High Performance Network Web Server Framework"
        LANGUAGES C CXX
        )

# Use Logger or not
IF (${LOG_LEVEL} MATCHES "NOLOG")
    MESSAGE("Build in ${LOG_LEVEL} mode without Logging")
    ADD_DEFINITIONS(-DNOLOG)
ELSE()
    MESSAGE("Build with Logging enabled")
ENDIF()

# Use Timer or not
IF (DEFINED TIMER)
    MESSAGE("Build using timer of expiration ${TIMER}")
    ADD_DEFINITIONS(-DTIMER_EXPIRATION=${TIMER})
ELSE()
    MESSAGE("Build without timer")
    ADD_DEFINITIONS(-DTIMER_EXPIRATION=0)
ENDIF()

# Find the required thread package on the platform
SET(THREADS_PREFER_PTHREAD_FLAG ON)
FIND_PACKAGE(Threads REQUIRED)

# Formatting utility search path
set(NEXT_SERVERTU_CLANG_SEARCH_PATH "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin" "/usr/local/opt/llvm@8/bin" "/usr/local/Cellar/llvm/8.0.1/bin")

# Includes header file path
SET(NEXT_SERVER_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
SET(NEXT_SERVER_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
SET(NEXT_MYSQL_INCLUDE_DIR "/usr/include/cppconn") # Linux only, might not be portable
SET(NEXT_SERVER_DEMO_DIR ${PROJECT_SOURCE_DIR}/demo)

# Build
######################################################################################################################
SET(CMAKE_COMPILER_FLAG -O3 -Wall -Wextra -pedantic -Werror)
SET(MYSQL_COMPILER_FLAG -I/usr/include/cppconn -L/usr/local/lib -lmysqlcppconn)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# Build the turtle core library
FILE(GLOB TURTLE_CORE_SOURCE RELATIVE ${CMAKE_SOURCE_DIR} "src/core/*.cpp")
ADD_LIBRARY(turtle_core ${TURTLE_CORE_SOURCE})
TARGET_LINK_LIBRARIES(turtle_core turtle_log Threads::Threads)
TARGET_COMPILE_OPTIONS(turtle_core PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        turtle_core
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the turtle logging library
FILE(GLOB TURTLE_LOG_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/log/*.cpp")
ADD_LIBRARY(turtle_log ${TURTLE_LOG_SOURCES})
TARGET_LINK_LIBRARIES(turtle_log Threads::Threads)
TARGET_COMPILE_OPTIONS(turtle_log PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        turtle_log
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the echo server
ADD_EXECUTABLE(echo_server ${NEXT_SERVER_DEMO_DIR}/echo/echo_server.cpp)
TARGET_LINK_LIBRARIES(echo_server turtle_core)
TARGET_COMPILE_OPTIONS(echo_server PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        echo_server
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the echo client
ADD_EXECUTABLE(echo_client ${NEXT_SERVER_DEMO_DIR}/echo/echo_client.cpp)
TARGET_LINK_LIBRARIES(echo_client turtle_core)
TARGET_INCLUDE_DIRECTORIES(
        echo_client
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)



