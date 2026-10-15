#include "vehicle.hpp"

#include <iostream>

int main() {
	tactical::Vehicle vehicle({
		.heading_degrees = 0.0,
		.speed_meters_per_second = 10.0,
		.operational_state = tactical::OperationalState::Active,
	});
	vehicle.update(1.0);

	const auto& state = vehicle.state();
	std::cout << "Vehicle position: (" << state.position.x_meters << ", "
			  << state.position.y_meters << ") meters\n"
		      << "Battery: " << state.battery_percent << "%\n";
	return 0;
}
