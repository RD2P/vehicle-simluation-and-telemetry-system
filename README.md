# C++ Tactical Simulation and Telemetry System

A C++20 simulation of unmanned ground vehicles with a focus on deterministic vehicle behavior, reliable state management, concurrency, and communications.

The project currently provides a single-vehicle simulation core. Vehicles track position, heading, speed, battery level, and operational state. Active vehicles move deterministically, consume battery, and become disabled when depleted. Lifecycle transitions and invalid inputs are validated, with behavior covered by CTest.

## Architecture

```text
+-----------------------------+
|       Vehicle Core          |
| State, movement, lifecycle, |
| and validation              |
+--------------+--------------+
               |
               v
+-----------------------------+
|     Simulation Services     |
| Concurrent updates and     |
| telemetry collection       |
+--------------+--------------+
               |
               v
+-----------------------------+
|       Control Systems       |
| Communication, monitoring,  |
| and operator interfaces     |
+-----------------------------+
```

## Build and Run

For a complete configure, build, test, and run cycle, use the helper script:

```bash
bash run.bash
```

The individual commands are also available when you need to run one step at a time.

Configure and build the project with CMake:

```bash
cmake -S . -B build
cmake --build build
```

Run the demonstration executable:

```bash
./build/cpp_tactical_simulation
```

Run the test suite:

```bash
ctest --test-dir build --output-on-failure
```

Remove generated build artifacts with:

```bash
cmake --build build --target clean
```

## Repository Layout

```text
src/vehicle.hpp       Vehicle state and public API
src/vehicle.cpp       Vehicle behavior and validation
src/main.cpp          Small executable demonstration
tests/vehicle_test.cpp Vehicle behavior tests
CMakeLists.txt        Build and test configuration
run.bash              Quick configure, build, test, and run script
```


