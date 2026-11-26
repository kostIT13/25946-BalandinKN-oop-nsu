#include "cargo_robot.h"

const std::string CargoRobot::class_name = "CargoRobot";

double CargoRobot::normalizeWeight(double value) {
    return value < 0.0 ? 0.0 : value;
}

void CargoRobot::announce(const std::string& what) const {
    std::cout << "      [" << class_name << "] " << what
              << " cargo=" << cargoWeight << " kg\n";
}

CargoRobot::CargoRobot(double cargoWeight, const std::string& name)
    : QuadrupedRobot(name), cargoWeight(normalizeWeight(cargoWeight))
{
    announce("constructor");
}

CargoRobot::~CargoRobot() {
    announce("destructor");
}
