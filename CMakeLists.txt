cmake_minimum_required(VERSION 3.30)
project(Project3Group22)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/dependencies/SFML/SFML-2.5.1/lib/cmake/SFML")
set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/curl/curl-8.15.0_4-win64-mingw/include")
set(CURL_LIBRARY "${CMAKE_SOURCE_DIR}/dependencies/curl/curl-8.15.0_4-win64-mingw/lib/libcurl.dll.a")

include_directories("${CMAKE_SOURCE_DIR}/dependencies/nlohmannjson")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
find_package(CURL REQUIRED)

add_executable(Project3Group22 src/main.cpp)
target_link_libraries(Project3Group22 PRIVATE CURL::libcurl sfml-system sfml-window sfml-graphics sfml-audio)

set(CURL_DLL_PATH "${CMAKE_SOURCE_DIR}/dependencies/curl/curl-8.15.0_4-win64-mingw/bin/libcurl-x64.dll")


# Copy all SFML DLLs to the output folder
add_custom_command(
        TARGET Project3Group22
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/dependencies/SFML/SFML-2.5.1/bin"
        $<TARGET_FILE_DIR:Project3Group22>
)

# Copy libcurl DLL to the output folder
add_custom_command(
        TARGET Project3Group22
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/dependencies/curl/curl-8.15.0_4-win64-mingw/bin/libcurl-x64.dll"
        $<TARGET_FILE_DIR:Project3Group22>
)