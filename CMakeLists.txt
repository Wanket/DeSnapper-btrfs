cmake_minimum_required(VERSION 3.10)
project(DeSnapper-btrfs C)

set(CMAKE_C_STANDARD 11)

execute_process(COMMAND sh -c "python3-config --includes --ldflags | tr '\\n' ' '" OUTPUT_VARIABLE PYTHON_CONFIG)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3")
set(CMAKE_C_FLAGS_RELEASE  "${CMAKE_C_FLAGS_RELEASE} -O2 -g -DNDEBUG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PYTHON_CONFIG} -fPIC -Wall -Wextra")

include_directories(/usr/include/)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME} SHARED btrfs.c)

include(GNUInstallDirs)

if(DEFINED DEB_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/${DEB_PREFIX}/usr/lib")

    install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_LIBRARY_ARCHITECTURE}")
else()
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
endif()
