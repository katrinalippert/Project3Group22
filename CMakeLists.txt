cmake_minimum_required(VERSION 3.30)
project(Project3Group22)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/dependencies/SFML/SFML-2.5.1/lib/cmake/SFML")
set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/curl/curl-8.15.0_4-win64-mingw/include/curl")
set(CURL_LIBRARY "${CMAKE_SOURCE_DIR}/dependencies/curl/curl-8.15.0_4-win64-mingw/lib/libcurl.a")

include_directories("${CMAKE_SOURCE_DIR}/dependencies/nlohmannjson")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
find_package(CURL REQUIRED)

add_executable(Project3Group22 src/main.cpp)
target_link_libraries(Project3Group22 PRIVATE CURL::libcurl sfml-system sfml-window sfml-graphics sfml-audio)
