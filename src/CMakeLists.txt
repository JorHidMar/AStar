add_library(AStarLib SHARED 
    ${CMAKE_CURRENT_SOURCE_DIR}/stateDataType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/board.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astar.cpp)


target_include_directories(AStarLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>)


message("Done config")

# install(TARGETS AStarLib 
#     EXPORT AStarLibConfig
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib
#     RUNTIME DESTINATION bin)


# install(DIRECTORY include/ DESTINATION include)

# install(EXPORT AStarLibConfig DESTINATION share/mylibrary/cmake)

# install(FILES README.md DESTINATION share/doc/mylibrary)
