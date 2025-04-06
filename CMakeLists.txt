# # Specify the minimum CMake version
# cmake_minimum_required(VERSION 3.10)
#
# # Define the project name and its version
# project(SFML_Project VERSION 1.0)
#
# # Set the C++ standard
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
#
# # Add SFML as a subdirectory
# # Assumes SFML is located in external/SFML relative to this CMakeLists.txt
# add_subdirectory(external/SFML)
#
# # Add the executable
# # Assumes your source files are in the src/ directory
# add_executable(${PROJECT_NAME} src/main.cpp)
#
# # Link SFML libraries to the project
# target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics)
#
# # Optionally include SFML headers
# target_include_directories(${PROJECT_NAME} PRIVATE external/SFML/include)
#

cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE sfml-graphics)
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
        VERBATIM)
endif()
