cmake_minimum_required(VERSION 3.15)
project(Qwen3TTS-ncnn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ncnn package
# If you installed ncnn system-wide, this works.
# Otherwise, set ncnn_DIR to point to your ncnn build/lib/cmake/ncnn
find_package(ncnn REQUIRED)

# Include directories
include_directories(${ncnn_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/qwen3_tts.cpp
    src/pipeline.cpp
)

# Create executable
add_executable(qwen3_tts_ncnn ${SOURCES})

# Link libraries
target_link_libraries(qwen3_tts_ncnn ${ncnn_LIBRARIES})

# Platform specific adjustments
if(WIN32)
    target_compile_definitions(qwen3_tts_ncnn PRIVATE _CRT_SECURE_NO_WARNINGS)
    # Copy DLLs automatically on Windows (Optional)
    # add_custom_command(TARGET qwen3_tts_ncnn POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_directory
    #     "${ncnn_DIR}/../../bin" $<TARGET_FILE_DIR:qwen3_tts_ncnn>)
endif()
