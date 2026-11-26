#pragma once
#include "robot.h"

class CargoRobot : public virtual QuadrupedRobot {
private:
    double cargoWeight;

    static double normalizeWeight(double value);
    void announce(const std::string& what) const;

public:
    static const std::string class_name;

    explicit CargoRobot(double cargoWeight = 0.0,
                        const std::string& name = "Unnamed-Cargo");
    ~CargoRobot();

    double getCargoWeight() const { return cargoWeight; }
    void setCargoWeight(double value) { cargoWeight = normalizeWeight(value); }
};
