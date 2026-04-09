cmake_minimum_required(VERSION 3.10)

project(LunarLander)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tady jsme zvedli verzi na 3.0
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio Network REQUIRED)

# Najde všechny .cpp soubory v aktuální složce
file(GLOB SOURCES "*.cpp")

add_executable(LunarLander ${SOURCES})

# V SFML 3 se používají modernější názvy cílů (targets) s předponou SFML::
target_link_libraries(LunarLander SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)

# Zkopíruje složku ASSETS do složky, kde se buildí program
add_custom_command(TARGET LunarLander POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/ASSETS"
    "$<TARGET_FILE_DIR:LunarLander>/ASSETS")