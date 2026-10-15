#include "vehicle.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {

void expect(bool condition, const char* message) {
	if (!condition) {
		throw std::runtime_error(message);
	}
}

bool nearly_equal(double left, double right) {
	return std::abs(left - right) < 1e-9;
}

void active_vehicle_moves_and_drains_battery() {
	tactical::Vehicle vehicle({
		.position = {},
		.heading_degrees = 0.0,
		.speed_meters_per_second = 10.0,
		.battery_percent = 100.0,
		.operational_state = tactical::OperationalState::Active,
	});

	vehicle.update(2.0);
	const auto& state = vehicle.state();

	expect(nearly_equal(state.position.x_meters, 20.0), "vehicle should move along the x axis");
	expect(nearly_equal(state.position.y_meters, 0.0), "vehicle should not move along the y axis");
	expect(nearly_equal(state.battery_percent, 98.0), "vehicle should drain two percent battery");
}

void idle_vehicle_does_not_move() {
	tactical::Vehicle vehicle({
		.position = {5.0, 7.0},
		.heading_degrees = 90.0,
		.speed_meters_per_second = 10.0,
		.battery_percent = 80.0,
		.operational_state = tactical::OperationalState::Idle,
	});

	vehicle.update(2.0);
	const auto& state = vehicle.state();

	expect(nearly_equal(state.position.x_meters, 5.0), "idle vehicle x position should not change");
	expect(nearly_equal(state.position.y_meters, 7.0), "idle vehicle y position should not change");
	expect(nearly_equal(state.battery_percent, 80.0), "idle vehicle battery should not drain");
}

void depleted_vehicle_becomes_disabled() {
	tactical::Vehicle vehicle({
		.speed_meters_per_second = 1.0,
		.battery_percent = 0.5,
		.operational_state = tactical::OperationalState::Active,
	});

	vehicle.update(1.0);

	expect(vehicle.state().operational_state == tactical::OperationalState::Disabled,
		"vehicle should be disabled when its battery reaches zero");
	expect(nearly_equal(vehicle.state().battery_percent, 0.0),
		"vehicle battery should not become negative");
}

void invalid_delta_time_is_rejected() {
	tactical::Vehicle vehicle;
	bool rejected = false;

	try {
		vehicle.update(-1.0);
	} catch (const std::invalid_argument&) {
		rejected = true;
	}

	expect(rejected, "negative delta time should be rejected");
}

}  // namespace

int main() {
	try {
		active_vehicle_moves_and_drains_battery();
		idle_vehicle_does_not_move();
		depleted_vehicle_becomes_disabled();
		invalid_delta_time_is_rejected();
	} catch (const std::exception& error) {
		std::cerr << "Vehicle tests failed: " << error.what() << '\n';
		return 1;
	}

	std::cout << "Vehicle tests passed\n";
	return 0;
}
