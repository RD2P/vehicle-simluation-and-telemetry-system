#include "vehicle.hpp"

#include <cmath>
#include <iostream>
#include <limits>
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

void lifecycle_transitions_are_explicit() {
	tactical::Vehicle vehicle;

	expect(vehicle.activate(), "idle vehicle should activate");
	expect(vehicle.state().operational_state == tactical::OperationalState::Active,
		"vehicle should be active after activation");
	expect(vehicle.deactivate(), "active vehicle should deactivate");
	expect(vehicle.state().operational_state == tactical::OperationalState::Idle,
		"vehicle should be idle after deactivation");
	expect(vehicle.disable(), "idle vehicle should be disableable");
	expect(vehicle.state().operational_state == tactical::OperationalState::Disabled,
		"vehicle should be disabled after disabling");
	expect(!vehicle.activate(), "disabled vehicle should not reactivate");
	expect(!vehicle.deactivate(), "disabled vehicle should not deactivate");
	expect(!vehicle.disable(), "disabling an already disabled vehicle should be a no-op");
}

void motion_setters_update_state() {
	tactical::Vehicle vehicle;

	vehicle.set_heading(90.0);
	vehicle.set_speed(25.0);

	expect(nearly_equal(vehicle.state().heading_degrees, 90.0),
		"heading setter should update heading");
	expect(nearly_equal(vehicle.state().speed_meters_per_second, 25.0),
		"speed setter should update speed");
}

void invalid_motion_values_are_rejected() {
	tactical::Vehicle vehicle;
	vehicle.set_heading(45.0);
	vehicle.set_speed(10.0);

	bool invalid_heading_rejected = false;
	try {
		vehicle.set_heading(std::numeric_limits<double>::infinity());
	} catch (const std::invalid_argument&) {
		invalid_heading_rejected = true;
	}

	bool invalid_speed_rejected = false;
	try {
		vehicle.set_speed(-1.0);
	} catch (const std::invalid_argument&) {
		invalid_speed_rejected = true;
	}

	expect(invalid_heading_rejected, "infinite heading should be rejected");
	expect(invalid_speed_rejected, "negative speed should be rejected");
	expect(nearly_equal(vehicle.state().heading_degrees, 45.0),
		"rejected heading should not change state");
	expect(nearly_equal(vehicle.state().speed_meters_per_second, 10.0),
		"rejected speed should not change state");
}

void empty_battery_cannot_be_active() {
	bool rejected = false;

	try {
		tactical::Vehicle vehicle({
			.battery_percent = 0.0,
			.operational_state = tactical::OperationalState::Active,
		});
	} catch (const std::invalid_argument&) {
		rejected = true;
	}

	expect(rejected, "active vehicle with empty battery should be rejected");
}

}  // namespace

int main() {
	try {
		active_vehicle_moves_and_drains_battery();
		idle_vehicle_does_not_move();
		depleted_vehicle_becomes_disabled();
		invalid_delta_time_is_rejected();
		lifecycle_transitions_are_explicit();
		motion_setters_update_state();
		invalid_motion_values_are_rejected();
		empty_battery_cannot_be_active();
	} catch (const std::exception& error) {
		std::cerr << "Vehicle tests failed: " << error.what() << '\n';
		return 1;
	}

	std::cout << "Vehicle tests passed\n";
	return 0;
}
