cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 11)
project(os1r LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Include the source directory recursively
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.h")

add_executable(os1r src/main.cpp)

target_link_libraries(os1r PRIVATE sfml-graphics)
target_compile_features(os1r PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET os1r
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:os1r>
        VERBATIM)
endif()

install(TARGETS os1r)