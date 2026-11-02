cmake_minimum_required(VERSION 3.20)
project(PowerRunner CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB SOURCES src/*.cpp)
add_executable(PowerRunner ${SOURCES})
target_include_directories(PowerRunner PRIVATE src)
target_link_libraries(PowerRunner PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    nlohmann_json::nlohmann_json)

# Copy assets next to the exe so it runs from VS Code or with a double-click
add_custom_command(TARGET PowerRunner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:PowerRunner>/assets)
