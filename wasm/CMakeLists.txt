cmake_minimum_required(VERSION 3.10)
project(mshv_ft8)

set(CMAKE_CXX_STANDARD 14)


# Add Emscripten-specific options
set(CMAKE_EXECUTABLE_SUFFIX ".js")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MODULARIZE=1 -s EXPORT_NAME='createMshvFT8Module'")

# Define a variable for debug mode: build.sh -d
option(DEBUG_MODE "Enable debug mode" OFF)

#qt5/qt6 (for QStrings and text)
#set(CMAKE_PREFIX_PATH "/usr/include/x86_64-linux-gnu/qt5")
#set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")
#set(Qt5Core_DIR "${CMAKE_PREFIX_PATH}/Qt5Core")
#set(Qt6Core_DIR "${CMAKE_PREFIX_PATH}/Qt6Core")
#find_package(Qt5Core REQUIRED)
#find_package(Qt6Core REQUIRED)

# Common flags
set(COMMON_FLAGS "-s WASM=1 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s ALLOW_MEMORY_GROWTH=1")

# Debug-specific flags
set(DEBUG_FLAGS "-s ASSERTIONS=2 -g -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2 -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.")

# Release-specific flags
set(RELEASE_FLAGS "-O3 -s WASM_BIGINT=1")

# Set flags based on debug mode
if(DEBUG_MODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} ${DEBUG_FLAGS}")
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} ${RELEASE_FLAGS}")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/wasm_specific)
include_directories(${CMAKE_SOURCE_DIR}/src/mshv_adapted)
#include_directories(${CMAKE_SOURCE_DIR}/src/qtext_simple)
include_directories(${CMAKE_SOURCE_DIR}/src/qt_minimal)
#include_directories(${Qt5Core_INCLUDE_DIRS})
#include_directories(${Qt6Core_INCLUDE_DIRS})

# Add source files
file(GLOB WASM_SPECIFIC_SOURCES "${CMAKE_SOURCE_DIR}/src/wasm_specific/*.cpp")
file(GLOB MSHV_ADAPTED_SOURCES "${CMAKE_SOURCE_DIR}/src/mshv_adapted/*.cpp")
file(GLOB MSHV_QTEXT_SOURCES "${CMAKE_SOURCE_DIR}/src/qtext_simple/*.cpp")
#file(GLOB QT_TEXT_SOURCES "${CMAKE_SOURCE_DIR}/../../src/qtbase/src/corelib/text/*.cpp")

add_executable(mshv_ft8
    ${WASM_SPECIFIC_SOURCES}
    ${MSHV_ADAPTED_SOURCES}
    ${MSHV_QTEXT_SOURCES}
    #${QT_TEXT_SOURCES}
)

# Link libraries
#target_link_libraries(mshv_ft8 Qt5::Core)
#include_directories(${Qt5Core_INCLUDE_DIRS})
#target_link_libraries(mshv_ft8 Qt6::Core)
#include_directories(${Qt6Core_INCLUDE_DIRS})

# Add exported functions
set_target_properties(mshv_ft8 PROPERTIES 
    OUTPUT_NAME "mshv_ft8" LINK_FLAGS "\
    -s EXPORTED_FUNCTIONS=['_init_ft8','_free','_pack_ft8_message','_pack_ft8_message_safe','_unpack_ft8_message','_save_hash_call']\
    -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','print','printErr','UTF8ToString','getValue','allocateUTF8']"
)

# Add debug symbols for all source files when in debug mode
if(DEBUG_MODE)
    target_compile_options(mshv_ft8 PRIVATE 
        -g 
        "$<$<COMPILE_LANGUAGE:CXX>:-gsource-map>"
    )
endif()