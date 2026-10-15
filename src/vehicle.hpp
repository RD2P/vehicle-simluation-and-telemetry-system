#pragma once

namespace tactical {

enum class OperationalState {
	Idle,
	Active,
	Disabled,
};

struct Position {
	double x_meters{0.0};
	double y_meters{0.0};
};

struct VehicleState {
	Position position{};
	double heading_degrees{0.0};
	double speed_meters_per_second{0.0};
	double battery_percent{100.0};
	OperationalState operational_state{OperationalState::Idle};
};

class Vehicle {
public:
	explicit Vehicle(VehicleState state = {});

	const VehicleState& state() const noexcept;
	void update(double delta_seconds);

private:
	VehicleState state_;
};

}  // namespace tactical
