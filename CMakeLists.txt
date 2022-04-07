cmake_minimum_required(VERSION 3.12)

project(nesemulator VERSION 1.0.0 LANGUAGES C CXX)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB source_files CONFIGURE_DEPENDS "./*/*.cpp")

add_executable(nesemulator ${source_files})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else()
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
endif()

target_link_libraries(nesemulator ${SDL2_LIBRARIES})