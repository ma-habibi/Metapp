cmake_minimum_required(VERSION 3.14)

project(Metapp VERSION)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  option (GIT_SUBMODULE "Check submodules during build" ON)
  if (GIT_SUBMODULE)
    message(STATUS "Adding submodule")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if (NOT GIT_SUBMOD_RESULT EQUAL 0)
      message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}")
    endif()
  endif()
endif()

add_subdirectory(external/yaml-cpp)
add_subdirectory(external/SQLiteCpp)
add_subdirectory(external/argparse)

add_executable(${PROJECT_NAME} main.cpp
  main.cpp
  Core.cpp
  Core.h
)

target_include_directories(${PROJECT_NAME} 
  PUBLIC external/yaml-cpp/include
  PUBLIC external/SQLiteCpp/include
  PUBLIC external/argparse/include
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC yaml-cpp::yaml-cpp
  PUBLIC SQLiteCpp
  PUBLIC argparse
)

target_compile_features(Metapp PRIVATE cxx_std_17)
