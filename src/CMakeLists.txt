add_library(AStarLib SHARED 
    ${CMAKE_CURRENT_SOURCE_DIR}/listState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/board.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astar.cpp)


target_include_directories(AStarLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>)
