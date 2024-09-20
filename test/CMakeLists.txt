
# Add test executable
add_executable(listStateTest listStateTest.cpp)
add_executable(boardContainerTest boardContainerTest.cpp)

# Link GoogleTest and the main project library to the test executable
target_link_libraries(listStateTest PRIVATE gtest gtest_main AStarLib)
target_link_libraries(boardContainerTest PRIVATE gtest gtest_main AStarLib)

# Define tests
add_test(NAME listState_test COMMAND 
    listStateTest)

# Define tests
add_test(NAME boardContainer_test COMMAND 
    boardContainerTest)
