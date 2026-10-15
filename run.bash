cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/cpp_tactical_simulation