cmake_minimum_required(VERSION 3.10)
project(mshv_ft8)

set(CMAKE_CXX_STANDARD 14)

# Add Emscripten-specific options
set(CMAKE_EXECUTABLE_SUFFIX ".js")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s ASSERTIONS=1 -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s ALLOW_MEMORY_GROWTH=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s MODULARIZE=1 -s EXPORT_NAME='createMshvFT8Module'")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/wasm_specific)
include_directories(${CMAKE_SOURCE_DIR}/src/mshv_adapted)

# Add source files
add_executable(mshv_ft8
    ${CMAKE_SOURCE_DIR}/src/wasm_specific/QString.cpp
    ${CMAKE_SOURCE_DIR}/src/wasm_specific/QChar.cpp
    ${CMAKE_SOURCE_DIR}/src/wasm_specific/QStringList.cpp
    ${CMAKE_SOURCE_DIR}/src/wasm_specific/packwrap.cpp
    ${CMAKE_SOURCE_DIR}/src/mshv_adapted/unpack_msg.cpp
    ${CMAKE_SOURCE_DIR}/src/mshv_adapted/pack_unpack_msg77.cpp
    ${CMAKE_SOURCE_DIR}/src/mshv_adapted/pack_msg.cpp
    ${CMAKE_SOURCE_DIR}/src/mshv_adapted/hvqthloc.cpp
)

# Link libraries
target_link_libraries(mshv_ft8)

# Add exported functions
set_target_properties(mshv_ft8 PROPERTIES 
    OUTPUT_NAME "mshv_ft8" LINK_FLAGS "\
    -s EXPORTED_FUNCTIONS=['_init_ft8','_free','_pack_ft8_message','_unpack_ft8_message','_save_hash_call']\
    -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','print','printErr','UTF8ToString','getValue','allocateUTF8']"
)

# 'stringToNewUTF8','UTF8ToString'