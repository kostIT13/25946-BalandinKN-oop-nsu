#include "cargo_quadruped_robot.h"

double CargoQuadrupedRobot::stepBatteryCost() const {
    return 1.0 + cargoWeightKg / 100.0 * extraConsumptionPerHundredKg;
}

CargoQuadrupedRobot::CargoQuadrupedRobot(const std::string& name,
                                         double battery, double cargoWeightKg,
                                         int extraConsumptionPerHundredKg)
    : QuadrupedRobot(name, battery, 50), cargoWeightKg(cargoWeightKg),
      extraConsumptionPerHundredKg(extraConsumptionPerHundredKg) {}

void CargoQuadrupedRobot::move() {
    const double cost = stepBatteryCost();
    takeStep(cost);
    logState("move: loaded step, cost=" + std::to_string(cost) +
             " per step, cargo=" + std::to_string(cargoWeightKg) + " kg");
}

std::string CargoQuadrupedRobot::className() const {
    return "CargoQuadrupedRobot";
}

std::string CargoQuadrupedRobot::gaitTag() const {
    return "loaded-trot";
}
