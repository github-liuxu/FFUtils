cmake_minimum_required(VERSION 3.30.0)
project(FFUtils VERSION 0.0.1)
enable_language(CXX OBJC OBJCXX)
add_executable(FFUtils FFUtils/main.cpp)

add_subdirectory(../FFReadWriter FFReadWriter)

target_compile_features(FFUtils PRIVATE cxx_std_11)

target_include_directories(FFUtils PRIVATE ../FFReadWriter/Sources/FFReadWriter/ffmpeg-sdk-osx/include)

target_link_libraries(FFUtils PRIVATE FFReadWriter
"-framework Foundation"
)

target_link_directories(FFUtils PRIVATE
    ${CMAKE_SOURCE_DIR}/build/FFReadWriter
    ${CMAKE_SOURCE_DIR}/../FFReadWriter/Sources/FFReadWriter/ffmpeg-sdk-osx/lib
)