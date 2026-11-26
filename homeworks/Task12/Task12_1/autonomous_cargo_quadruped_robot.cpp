#include "autonomous_cargo_quadruped_robot.h"

double AutonomousCargoQuadrupedRobot::stepBatteryCost() const {
    return 0.5 + getCargoWeightKg() / 100.0 * 1;
}

AutonomousCargoQuadrupedRobot::AutonomousCargoQuadrupedRobot(
    const std::string& name, double battery, double cargoWeightKg,
    double autonomousRangeM)
    : CargoQuadrupedRobot(name, battery, cargoWeightKg, 3),
      autonomousRangeM(autonomousRangeM < 0.0 ? 0.0 : autonomousRangeM) {}

void AutonomousCargoQuadrupedRobot::move() {
    if (autonomousRangeM <= 0.0) {
        logState("move: autonomous range exhausted, standing by");
        return;
    }
    const double cost = stepBatteryCost();
    takeStep(cost);
    autonomousRangeM -= getGaitStrideCm() / 100.0;
    if (autonomousRangeM < 0.0) autonomousRangeM = 0.0;
    logState("move: autonomous march, left=" + std::to_string(autonomousRangeM) +
             " m, cost=" + std::to_string(cost));
}

std::string AutonomousCargoQuadrupedRobot::className() const {
    return "AutonomousCargoQuadrupedRobot";
}

std::string AutonomousCargoQuadrupedRobot::gaitTag() const {
    return "autonomous-march";
}
