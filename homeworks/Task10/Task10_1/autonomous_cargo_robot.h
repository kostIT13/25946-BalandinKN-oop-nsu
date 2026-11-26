#pragma once
#include "cargo_robot.h"

#include <string>

class AutonomousCargoRobot final : private CargoRobot {
private:
    bool autonomousMode;

    void printMsg(const std::string& what) const;

public:
    static const std::string class_name;

    AutonomousCargoRobot();
    AutonomousCargoRobot(const std::string& id, const std::string& name,
                         double battery, const std::string& command,
                         double dist, const double* last10Arr,
                         double cargoWeight,
                         double maxCargoWeight = CargoRobot::defaultMaxCargo(),
                         bool autonomousMode = false);

    AutonomousCargoRobot(const AutonomousCargoRobot& o);
    AutonomousCargoRobot& operator=(const AutonomousCargoRobot& o);
    AutonomousCargoRobot(AutonomousCargoRobot&& o) noexcept;
    AutonomousCargoRobot& operator=(AutonomousCargoRobot&& o) noexcept;
    ~AutonomousCargoRobot();

    using CargoRobot::show_weight;
    using CargoRobot::getCargoWeight;
    using CargoRobot::setCargoWeight;

    void reportMode();
    void printAutonomousRobot() const;

    bool isAutonomousMode() const { return autonomousMode; }
    void setAutonomousMode(bool on) { autonomousMode = on; }
    std::string modeName() const {
        return autonomousMode ? "autonomous" : "controlled";
    }
};
