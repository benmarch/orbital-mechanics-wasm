cmake_minimum_required(VERSION 3.3)
project(orbital_mechanics)

# ensure emscripten header files are included
include_directories( $ENV{EMSCRIPTEN}/system/include/ )

# configure the project
set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(TARGET om.js)
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/wasm)
SET(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/web)

# get all source files
aux_source_directory(${SOURCE_DIR} SOURCE_FILES)

# get all resource files (this is really just so the CLion recognizes them)
file(GLOB_RECURSE RESOURCE_FILES ${RESOURCE_DIR}/*.*)

# register the target
add_executable(orbital_mechanics ${SOURCE_FILES} ${RESOURCE_FILES})

# configure the output
set_target_properties(orbital_mechanics
        PROPERTIES
        RESOURCE "${RESOURCE_FILES}"
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
        RUNTIME_OUTPUT_NAME ${TARGET}
        )

# system magic
if(NOT EMSCRIPTEN_PREFIX)
    if(DEFINED ENV{EMSCRIPTEN})
        set(EMSCRIPTEN_PREFIX $ENV{EMSCRIPTEN})
    else()
        set(EMSCRIPTEN_PREFIX "/usr/lib/emscripten")
    endif()
endif()

# more system magic
if(CMAKE_HOST_WIN32)
    set(EMCC_SUFFIX ".bat")
else()
    set(EMCC_SUFFIX "")
endif()

# configure the compilers
set(CMAKE_C_COMPILER "${EMSCRIPTEN_PREFIX}/emcc${EMCC_SUFFIX}")
set(CMAKE_CXX_COMPILER "${EMSCRIPTEN_PREFIX}/em++${EMCC_SUFFIX}")
set(CMAKE_AR "${EMSCRIPTEN_PREFIX}/emar${EMCC_SUFFIX}" CACHE FILEPATH "Emscripten ar")
set(CMAKE_RANLIB "${EMSCRIPTEN_PREFIX}/emranlib${EMCC_SUFFIX}" CACHE FILEPATH "Emscripten ranlib")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_CXX_FLAGS "-std=c++1y -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 --bind -Wall -O3 -DNDEBUG -o ${TARGET}")

set(CMAKE_VERBOSE_MAKEFILE on)

# ensure output directory exists before building
add_custom_command(TARGET orbital_mechanics PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
        )

# copy resources after build
add_custom_command(TARGET orbital_mechanics POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_DIR} ${OUTPUT_DIR}
        )
