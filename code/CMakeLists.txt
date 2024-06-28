cmake_minimum_required(VERSION 3.10)
project(compilador VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-g -Og -Wall -pedantic -O3")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

add_executable(main src/main.cpp)

add_library(ImageProcessor src/ImageProcessor.cpp)
target_include_directories(ImageProcessor PUBLIC include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(ImageProcessor ${OpenCV_LIBS})

add_library(SudokuBoard src/SudokuBoard.cpp)
target_include_directories(SudokuBoard PUBLIC include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(SudokuBoard ${OpenCV_LIBS})

add_library(SudokuSolver src/SudokuSolver.cpp)
target_include_directories(SudokuSolver PUBLIC include)
target_link_libraries(SudokuSolver SudokuBoard)


target_link_libraries(main SudokuBoard SudokuSolver ImageProcessor ${OpenCV_LIBS})
