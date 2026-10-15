#include "vehicle.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace tactical {

namespace {

constexpr double kBatteryDrainPercentPerSecond = 1.0;

void validate_state(const VehicleState& state) {
	if (!std::isfinite(state.position.x_meters) ||
		!std::isfinite(state.position.y_meters) ||
		!std::isfinite(state.heading_degrees) ||
		!std::isfinite(state.speed_meters_per_second) ||
		!std::isfinite(state.battery_percent)) {
		throw std::invalid_argument("vehicle state values must be finite");
	}

	if (state.speed_meters_per_second < 0.0) {
		throw std::invalid_argument("vehicle speed cannot be negative");
	}

	if (state.battery_percent < 0.0 || state.battery_percent > 100.0) {
		throw std::invalid_argument("vehicle battery must be between 0 and 100 percent");
	}
}

}  // namespace

Vehicle::Vehicle(VehicleState state) : state_(state) {
	validate_state(state_);
}

const VehicleState& Vehicle::state() const noexcept {
	return state_;
}

void Vehicle::update(double delta_seconds) {
	if (!std::isfinite(delta_seconds) || delta_seconds < 0.0) {
		throw std::invalid_argument("delta time must be finite and non-negative");
	}

	if (state_.operational_state != OperationalState::Active || delta_seconds == 0.0) {
		return;
	}

	const double heading_radians = state_.heading_degrees * (3.14159265358979323846 / 180.0);
	state_.position.x_meters += std::cos(heading_radians) * state_.speed_meters_per_second * delta_seconds;
	state_.position.y_meters += std::sin(heading_radians) * state_.speed_meters_per_second * delta_seconds;

	state_.battery_percent =
		std::max(0.0, state_.battery_percent - kBatteryDrainPercentPerSecond * delta_seconds);
	if (state_.battery_percent == 0.0) {
		state_.operational_state = OperationalState::Disabled;
	}
}

}  // namespace tactical
