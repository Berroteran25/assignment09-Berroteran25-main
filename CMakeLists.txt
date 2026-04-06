cmake_minimum_required( VERSION 3.10 )
project(exercise LANGUAGES CXX )

# require a C++17 compiler for all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp code.h code.cpp)
target_compile_options(main PRIVATE -Werror -Wall -Wextra -Wconversion -Wnarrowing -Wpedantic -Wno-sign-compare -Wno-unused-variable)

# add Catch2 library
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests tests.cpp code.h code.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_compile_options(tests PRIVATE -Werror -Wall -Wextra -Wconversion -Wnarrowing -Wpedantic -Wno-sign-compare -Wno-unused-variable)

set(destinations Atlanta Boston Chicago Dallas Seattle)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:main>/destinations
)

foreach(city ${destinations})
    add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/destinations/${city}.csv
                $<TARGET_FILE_DIR:main>/destinations/${city}.csv
    )
endforeach()


# -Werror: Treat warnings as errors. Without this flag a warning is generated when compiling, but you might miss it.
# -Wall: A better name would be -Wcommon. It enables common warnings like use of uninitialized variables.
# -Wextra: Some more common warnings not enabled by -Wall.
# -Wconversion: Enables warnings about conversions that might change the value like float to int.
# -Wpedantic: This enables strict standard conformance.
# -Wno-sign-compare: This disables warnings/errors for unsigned integers comparisons in for loops.
# -Wno-unused-variable: This disables warnings/errors for unused variables.