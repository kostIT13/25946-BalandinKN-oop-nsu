#pragma once
#include "autonomous_robot.h"
#include "cargo_robot.h"

class CargoAutonomousRobot : public CargoRobot, public AutonomousRobot {
private:
    void announce(const std::string& what) const;

public:
    static const std::string class_name;

    CargoAutonomousRobot(double cargoWeight, double autonomousRangeKm,
                         const std::string& name);
    ~CargoAutonomousRobot();

    void describe() const;
};
