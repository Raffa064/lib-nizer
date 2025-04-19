mkdir -p build && cd build

cmake .. && cmake --build .

g++ ../test/main.cpp libnizer.a -I../include -o test -g

echo
./test
