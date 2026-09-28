cmake_minimum_required(VERSION 3.20)
project(Week3WorkflowLab LANGUAGES CXX)

option(LAB_ENABLE_SANITIZERS "Enable AddressSanitizer and UBSan" OFF)
option(LAB_WITH_GTEST "Build additional GoogleTest tests using an installed package" OFF)

add_library(lab_options INTERFACE)
target_compile_features(lab_options INTERFACE cxx_std_17)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lab_options INTERFACE -Wall -Wextra -Wpedantic)
    if(LAB_ENABLE_SANITIZERS)
        target_compile_options(lab_options INTERFACE
            -fsanitize=address,undefined -fno-omit-frame-pointer
            -fno-sanitize-recover=undefined)
        target_link_options(lab_options INTERFACE
            -fsanitize=address,undefined -fno-omit-frame-pointer
            -fno-sanitize-recover=undefined)
    endif()
elseif(LAB_ENABLE_SANITIZERS)
    message(FATAL_ERROR "This lab's sanitizer configuration needs GCC or Clang on Linux/WSL")
endif()

add_library(material src/material.cpp)
target_include_directories(material PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(material PUBLIC lab_options)

add_executable(material_demo app/main.cpp)
target_link_libraries(material_demo PRIVATE material)

include(CTest)
if(BUILD_TESTING)
    add_executable(material_tests tests/material_tests.cpp)
    target_link_libraries(material_tests PRIVATE material)
    foreach(case_name IN ITEMS normal boundary invalid repeated)
        add_test(NAME material.${case_name} COMMAND material_tests ${case_name})
    endforeach()
    if(LAB_WITH_GTEST)
        find_package(GTest CONFIG REQUIRED)
        add_executable(material_gtests tests/material_gtest.cpp)
        target_link_libraries(material_gtests PRIVATE material GTest::gtest_main)
        include(GoogleTest)
        gtest_discover_tests(material_gtests TEST_PREFIX "gtest.")
    endif()
endif()

# Deliberately faulty programs: explicit build only, never registered with CTest.
foreach(fault_name IN ITEMS uaf oob overflow)
    add_executable(fault_${fault_name} EXCLUDE_FROM_ALL faults/${fault_name}.cpp)
    target_link_libraries(fault_${fault_name} PRIVATE lab_options)
endforeach()
