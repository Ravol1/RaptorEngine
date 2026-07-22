cmake_minimum_required(VERSION 4.2)
project(raptor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRECTORIES ./)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
add_executable(raptor ${SRCS})

target_include_directories(raptor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(re2 REQUIRED)
target_link_libraries(raptor PRIVATE re2::re2)

find_package(lexbor REQUIRED)
target_link_libraries(raptor PRIVATE lexbor::lexbor)

find_package(SDL3 REQUIRED)
target_link_libraries(raptor PRIVATE SDL3::SDL3)

find_package(SDL3_image REQUIRED)
target_link_libraries(raptor PRIVATE SDL3_image::SDL3_image)
