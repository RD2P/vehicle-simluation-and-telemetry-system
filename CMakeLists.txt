cmake_minimum_required(VERSION 3.20)

project(cpp_tactical_simulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cpp_tactical_simulation
	src/main.cpp
	src/vehicle.cpp
)

target_include_directories(cpp_tactical_simulation PRIVATE src)

include(CTest)

if(BUILD_TESTING)
	add_executable(vehicle_tests
		tests/vehicle_test.cpp
		src/vehicle.cpp
	)

	target_include_directories(vehicle_tests PRIVATE src)
	add_test(NAME vehicle_tests COMMAND vehicle_tests)
endif()
