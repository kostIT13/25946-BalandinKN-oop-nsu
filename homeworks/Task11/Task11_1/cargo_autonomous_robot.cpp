#include "cargo_autonomous_robot.h"

const std::string CargoAutonomousRobot::class_name = "CargoAutonomousRobot";

void CargoAutonomousRobot::announce(const std::string& what) const {
    std::cout << "      [" << class_name << "] " << what
              << " (most derived: it initializes the virtual base)\n";
}

CargoAutonomousRobot::CargoAutonomousRobot(double cargoWeight,
                                           double autonomousRangeKm,
                                           const std::string& name)
    : QuadrupedRobot(name),
      CargoRobot(cargoWeight, name),
      AutonomousRobot(autonomousRangeKm, name)
{
    announce("constructor");
}

CargoAutonomousRobot::~CargoAutonomousRobot() {
    announce("destructor");
}

void CargoAutonomousRobot::describe() const {
    std::cout << "  CargoAutonomousRobot: name=\"" << getName()
              << "\" cargo=" << getCargoWeight()
              << " kg range=" << getAutonomousRangeKm() << " km\n";
}
