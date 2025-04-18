# Set the cmake minimum version
cmake_minimum_required(VERSION 3.5)

# Define the project name
project(pascal--)

# Generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to C++17
set(CMAKE_CXX_STANDARD 17)

# Set source files
set(SOURCES
    src/lexical/Lexeme.cpp
    src/lexical/TokenType.cpp
    src/lexical/SymbolTable.cpp
    src/lexical/Lexer.cpp
    src/main.cpp
    src/util/File.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add all headers files under the include directory
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Add compilation flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# Enable better debugging information
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
endif()
