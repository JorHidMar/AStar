project(Astar)
cmake_minimum_required(VERSION 2.8.11)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(examples)

set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation directory" FORCE)

set(CPACK_PACKAGE_NAME "AStarLib")
set(CPACK_VERBATIM_VARIABLES true)
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jorge <>")


install(TARGETS AStarLib 
    EXPORT AStarLibConfig
    ARCHIVE DESTINATION local/lib
    LIBRARY DESTINATION local/lib
    RUNTIME DESTINATION local/bin)


install(DIRECTORY include/ DESTINATION local/include/AStarSimple)

install(EXPORT AStarLibConfig DESTINATION local/share/AStarSimple/cmake)

include(CPack)
