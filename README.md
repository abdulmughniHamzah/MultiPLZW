# This c++ project code implements our submitted paper "MultiPLZW: A Novel Multiple Pattern Matching Search in LZW-Compressed Data".

 The entry point to this project is the file search.cpp

 The data are supposed to be under the directory < /test-data>

# Commands:
***********
compile: g++ -g  search.cpp -o output.out -std=c++11
====================================================
run: ./output.out
====================================================
profiling execution time: time ./output.out
====================================================
memory profiling: valgrind --tool=massif  ./output.out
Note--->to profile memory usage according to allocate/dealocate operations use option <--time-unit=B> as:
valgrind --tool=massif --time-unit=B ./output.out
======================================================
