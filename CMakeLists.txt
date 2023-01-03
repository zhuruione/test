cmake_minimum_required(VERSION 3.19)
project(test)

set(CMAKE_CXX_STANDARD 14)
link_directories(/home/zr/share)
include_directories(/home/zr/share)

aux_source_directory(inotify_c INO_C)
aux_source_directory(color COLOR)

add_executable(test main.cpp ${INO_C} ${COLOR})
target_link_libraries(test mytime )

add_executable(mysql_test mysql_test.cpp)

add_executable(inotify_test inotify_test.cpp color/myout.cpp color/myout.h)
target_link_libraries(inotify_test mytime)
