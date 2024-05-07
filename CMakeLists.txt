cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")

project(wingsofvi-reader C)

set(VERSION "1.0.0")

file(GLOB SOURCES "src/*.c")

include(FetchContent)
FetchContent_Declare(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG        51b7f2abdade71cd9bb0e7a373ef2610ec6f9daf # v1.3.1
)
FetchContent_MakeAvailable(zlib)
# i thought this would work, but it doesn't. i guess having global CMAKE_C_FLAGS
# for -m32 isn't too bad anyways though
# target_compile_options(zlib PRIVATE -m32)

add_compile_options(-Wall -g)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} zlib)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include)

