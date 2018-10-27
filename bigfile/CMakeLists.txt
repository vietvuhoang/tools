cmake_minimum_required (VERSION 2.6)

project (bigfile)

file(GLOB Build_BigFile "*.cpp" "*.c")

add_executable(genbigfile ${Build_BigFile})
