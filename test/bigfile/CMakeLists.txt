cmake_minimum_required (VERSION 2.6)
project (test_bigfile C CXX)

find_package ( Threads REQUIRED )

set(SOURCE_CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../bigfile)

include_directories(
	"${SOURCE_CODE_DIR}"
)

file(GLOB SOURCE "*.c" "*.cpp" "${SOURCE_CODE_DIR}/*.cpp" "${SOURCE_CODE_DIR}/*.c")
list(REMOVE_ITEM SOURCE "${SOURCE_CODE_DIR}/big-file.cpp")

link_directories(${USER_LIBRARY_PATH})
add_executable(test_bigfile ${SOURCE} ${SOURCE_CODE_DIR})
target_link_libraries(test_bigfile gtest gmock gcov pthread )
target_compile_options(test_bigfile PRIVATE -fprofile-arcs -ftest-coverage -O3 )
