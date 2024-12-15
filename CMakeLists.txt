cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
project(os1r LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Include the source directory recursively

file(GLOB_RECURSE SOURCES "src/*.cpp")
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(os1r ${SOURCES}
        src/entities/EntityV3.h
        src/core/Artist.h
        src/core/SfmlArtist.h
        src/core/SfmlArtist.cpp
        src/core/TextureStore.h
        src/core/SfmlTextureStore.cpp
        src/core/SfmlTextureStore.h
)

target_include_directories(os1r PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(os1r PRIVATE sfml-graphics sfml-system sfml-audio)
target_compile_features(os1r PRIVATE cxx_std_11)

add_custom_command(
    TARGET os1r
    COMMENT "Copy content folder"
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E 
        copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/content $<TARGET_FILE_DIR:os1r>/content
    VERBATIM)

if(WIN32)
    add_custom_command(
        TARGET os1r
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E 
            copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:os1r>
        VERBATIM)
endif()

install(TARGETS os1r)