cmake_minimum_required(VERSION 3.13)
project(stb_image_lib C)
add_library(stb_image_lib SHARED
        stb.h
        stb_image.h
        stb_image_write.h)