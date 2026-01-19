cmake_minimum_required(VERSION 3.14)

project(Metapp VERSION 0.1)

add_subdirectory(external/yaml-cpp)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${POJECT_NAME}/.git")
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

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} 
  PUBLIC ${PROJECT_BINARY_DIR}
)

target_include_directories(${PROJECT_NAME} 
  PUBLIC external/yaml-cpp/include
)

target_link_directories(${PROJECT_NAME} 
  PRIVATE external/yaml-cpp/src
)

target_link_libraries(${PROJECT_NAME} PUBLIC yaml-cpp::yaml-cpp)
