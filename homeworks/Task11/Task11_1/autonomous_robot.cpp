#include "autonomous_robot.h"

const std::string AutonomousRobot::class_name = "AutonomousRobot";

double AutonomousRobot::normalizeRange(double value) {
    return value < 0.0 ? 0.0 : value;
}

void AutonomousRobot::announce(const std::string& what) const {
    std::cout << "      [" << class_name << "] " << what
              << " range=" << autonomousRangeKm << " km\n";
}

AutonomousRobot::AutonomousRobot(double autonomousRangeKm,
                                 const std::string& name)
    : QuadrupedRobot(name), autonomousRangeKm(normalizeRange(autonomousRangeKm))
{
    announce("constructor");
}

AutonomousRobot::~AutonomousRobot() {
    announce("destructor");
}
