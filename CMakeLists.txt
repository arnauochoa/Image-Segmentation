cmake_minimum_required(VERSION 3.13)
project(Image_Segmentation C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/BUILD")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/BUILD")

set(C_SOURCE_FILES
        src/main.c
        src/build_image_matrix.c src/build_image_matrix.h
        src/stb-master/stb_image.h
        src/stb-master/stb_image_write.h
        src/image.c src/image.h
        src/write_image_matrix.c src/write_image_matrix.h
        src/genetic_algorithm.c src/genetic_algorithm.h)

add_executable(${PROJECT_NAME} ${C_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} m)