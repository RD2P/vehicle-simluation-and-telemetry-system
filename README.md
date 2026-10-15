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

