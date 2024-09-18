
# Add test executable
add_executable(listStateTest listStateTest.cpp)

# Link GoogleTest and the main project library to the test executable
target_link_libraries(listStateTest PRIVATE gtest gtest_main AStarLib)

# Define tests
add_test(NAME listStateTest COMMAND listStateTest)
