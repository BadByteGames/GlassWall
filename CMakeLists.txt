cmake_minimum_required(VERSION 3.11.0)

project(GlassWall VERSION 1.0.0 LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)

find_package(OpenGL REQUIRED)

add_subdirectory(GlassWall)