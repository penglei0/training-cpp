cmake_minimum_required(VERSION 3.22)

project(training)

###########################################################
## Disallow building in the source directory
###########################################################
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
    FATAL: In-source builds are not allowed.
           You should create a separate directory for build files.
           ")
endif()

# Generating a compilation database: compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###########################################################
## C++17 Support Check
###########################################################
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if (COMPILER_SUPPORTS_CXX17)
    message(STATUS "c++17 is supported by the compiler")
else ()
    message(FATAL_ERROR "
            FATAL: the compiler does not support c++17")
endif ()

###########################################################
## specify the C++ standard
###########################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "CMAKE Version: ${CMAKE_VERSION}")
message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Host System name: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "System version: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
###########################################################
## Default built type (Release)
###########################################################
# change it to Debug by `-DCMAKE_BUILD_TYPE=Debug`
# Acceptable build types: Debug, Release, RelWithDebInfo, MinSizeRel
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()
message("Build Type: ${CMAKE_BUILD_TYPE}")

###########################################################
## Compiler flags
###########################################################
# Set default compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Set build-specific compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror -O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -O3")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(training
   training.cc
)
target_link_libraries(training 
    PRIVATE
    Threads::Threads)

add_executable(crtp_example
crtp_example.cc
 )
target_link_libraries(crtp_example 
    PRIVATE
    Threads::Threads)
# wrapper_ref
add_executable(wrapper_ref
    wrapper_ref.cc
    )
target_link_libraries(crtp_example 
    PRIVATE
    Threads::Threads)
    