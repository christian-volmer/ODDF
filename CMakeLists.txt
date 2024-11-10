cmake_minimum_required(VERSION 3.15)

project(oddf)

if(MSVC)
  add_compile_options(/W4 /w15038)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Weffc++)

  # Enforce C++17 to ensure we are not using features from later standards
  add_compile_options(-std=c++17)
endif()

add_subdirectory(lib/oddf)
add_subdirectory(lib/oddf_core)
add_subdirectory(lib/oddf_simulator_common)
add_subdirectory(lib/verilog)

add_subdirectory(examples)

# enable_testing()
# add_test(NAME self-test COMMAND benchmark --self-test)
