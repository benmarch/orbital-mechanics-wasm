cmake_minimum_required(VERSION 3.3)
project(orbital_mechanics)

include_directories( $ENV{EMSCRIPTEN}/system/include/ )

aux_source_directory(src/wasm SOURCE_FILES)
aux_source_directory(src/web RESOURCE_FILES)
add_executable(orbital_mechanics ${SOURCE_FILES})

if(NOT EMSCRIPTEN_PREFIX)
    if(DEFINED ENV{EMSCRIPTEN})
        set(EMSCRIPTEN_PREFIX $ENV{EMSCRIPTEN})
    else()
        set(EMSCRIPTEN_PREFIX "/usr/lib/emscripten")
    endif()
endif()

if(CMAKE_HOST_WIN32)
    set(EMCC_SUFFIX ".bat")
else()
    set(EMCC_SUFFIX "")
endif()

set(CMAKE_C_COMPILER "${EMSCRIPTEN_PREFIX}/emcc${EMCC_SUFFIX}")
set(CMAKE_CXX_COMPILER "${EMSCRIPTEN_PREFIX}/em++${EMCC_SUFFIX}")
set(CMAKE_AR "${EMSCRIPTEN_PREFIX}/emar${EMCC_SUFFIX}" CACHE FILEPATH "Emscripten ar")
set(CMAKE_RANLIB "${EMSCRIPTEN_PREFIX}/emranlib${EMCC_SUFFIX}" CACHE FILEPATH "Emscripten ranlib")

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set_target_properties(orbital_mechanics PROPERTIES
        RESOURCE "${RESOURCE_FILES}"
        )
file(GLOB_RECURSE CORE_HDR src/.h)
file(GLOB_RECURSE CORE_SRC src/.cpp)

set(TARGET build/om.js)

set(CMAKE_CXX_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 --bind -Wall -O3 -DNDEBUG -o ${TARGET}")

#[[
add_custom_command(TARGET orbital_mechanics POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy src/web/index.html build/index.html
        )]]
