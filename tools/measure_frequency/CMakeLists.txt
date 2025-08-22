cmake_minimum_required(VERSION 3.20)
project(measure_frequency VERSION 1.0)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
  add_compile_options(-Wall -Wextra -Werror=conversion -Werror=unused-but-set-variable -Werror=return-type -Werror=shadow)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSERIAL REQUIRED IMPORTED_TARGET libserial)

add_executable(measure_serial_freq serial_freq.cpp)

# Include and link
target_include_directories(measure_serial_freq PRIVATE ${LIBSERIAL_INCLUDE_DIRS})
target_link_libraries(measure_serial_freq ${LIBSERIAL_LIBRARIES})

# Add compiler flags from pkg-config
target_compile_options(measure_serial_freq PRIVATE ${LIBSERIAL_CFLAGS_OTHER})