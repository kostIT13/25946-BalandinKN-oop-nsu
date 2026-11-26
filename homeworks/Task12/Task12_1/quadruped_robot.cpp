#include "quadruped_robot.h"

double QuadrupedRobot::stepBatteryCost() const {
    return 1.0;
}

void QuadrupedRobot::takeStep(double cost) {
    distanceCoveredKm += gaitStrideCm / 100000.0;
    setBatteryLevel(getBatteryLevel() - cost);
}

QuadrupedRobot::QuadrupedRobot(const std::string& name, double battery,
                               int gaitStrideCm)
    : Robot(name, battery), gaitStrideCm(gaitStrideCm), distanceCoveredKm(0.0) {}

void QuadrupedRobot::move() {
    takeStep(stepBatteryCost());
    logState("move: gallop step");
}

std::string QuadrupedRobot::className() const {
    return "QuadrupedRobot";
}

std::string QuadrupedRobot::gaitTag() const {
    return "gallop";
}
