# setup
cmake_minimum_required(VERSION 3.11)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
set(NEXT_SERVER_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(NEXT_SERVERTU_CLANG_SEARCH_PATH "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin" "/usr/local/opt/llvm@8/bin" "/usr/local/Cellar/llvm/8.0.1/bin")

# clang-format
IF (NOT DEFINED CLANG_FORMAT_BIN)
    # attempt to find the binary if user did not specify
    FIND_PROGRAM(CLANG_FORMAT_BIN
            NAMES clang-format clang-format-14
            HINTS ${NEXT_SERVERTU_CLANG_SEARCH_PATH})
ENDIF ()
IF ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    MESSAGE(WARNING "Server/main couldn't find clang-format.")
ELSE ()
    MESSAGE(STATUS "Server/main found clang-format at ${CLANG_FORMAT_BIN}")
ENDIF ()

# cpplint
FIND_PROGRAM(CPPLINT_BIN
        NAMES cpplint cpplint.py
        HINTS ${NEXT_SERVER_BUILD_SUPPORT_DIR})
IF ("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
    MESSAGE(WARNING "Server/main couldn't find cpplint.")
ELSE ()
    MESSAGE(STATUS "Server/main found cpplint at ${CPPLINT_BIN}")
ENDIF ()

# clang-tidy
IF (NOT DEFINED CLANG_TIDY_BIN)
FIND_PROGRAM(CLANG_TIDY_BIN
        NAMES clang-tidy clang-tidy-14
        HINTS ${NEXT_SERVERTU_CLANG_SEARCH_PATH})
ENDIF ()
IF ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    MESSAGE(WARNING "Server/main couldn't find clang-tidy.")
ELSE ()
    SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)
    MESSAGE(STATUS "Server/main found clang-tidy at ${CLANG_TIDY_BIN}")
ENDIF ()

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

# Build the core library
FILE(GLOB NEXT_CORE_SOURCE RELATIVE ${CMAKE_SOURCE_DIR} "src/core/*.cpp")
ADD_LIBRARY(next_core ${NEXT_CORE_SOURCE})
TARGET_LINK_LIBRARIES(next_core next_log Threads::Threads)
TARGET_COMPILE_OPTIONS(next_core PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        next_core
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the http library
FILE(GLOB NEXT_HTTP_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/http/*.cpp")
ADD_LIBRARY(next_http ${NEXT_HTTP_SOURCES})
TARGET_LINK_LIBRARIES(next_http next_log)
TARGET_COMPILE_OPTIONS(next_http PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        next_http
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the logging library
FILE(GLOB next_log_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/log/*.cpp")
ADD_LIBRARY(next_log ${next_log_SOURCES})
TARGET_LINK_LIBRARIES(next_log Threads::Threads)
TARGET_COMPILE_OPTIONS(next_log PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        next_log
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the echo server
ADD_EXECUTABLE(echo_server ${NEXT_SERVER_DEMO_DIR}/echo/echo_server.cpp)
TARGET_LINK_LIBRARIES(echo_server next_core)
TARGET_COMPILE_OPTIONS(echo_server PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        echo_server
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the echo client
ADD_EXECUTABLE(echo_client ${NEXT_SERVER_DEMO_DIR}/echo/echo_client.cpp)
TARGET_LINK_LIBRARIES(echo_client next_core)
TARGET_INCLUDE_DIRECTORIES(
        echo_client
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)

# Build the http server
ADD_EXECUTABLE(http_server ${NEXT_SERVER_SRC_DIR}/http/http_server.cpp)
TARGET_LINK_LIBRARIES(http_server next_core next_http)
TARGET_COMPILE_OPTIONS(http_server PRIVATE ${CMAKE_COMPILER_FLAG})
TARGET_INCLUDE_DIRECTORIES(
        http_server
        PUBLIC ${NEXT_SERVER_SRC_INCLUDE_DIR}
)



