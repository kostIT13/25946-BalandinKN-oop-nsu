#pragma once
#include "robot.h"

class DiamondCargo : public QuadrupedRobot {
private:
    double cargoWeight;

public:
    DiamondCargo(double cargoWeight, const std::string& name);
    ~DiamondCargo();

    double getCargoWeight() const { return cargoWeight; }
};

class DiamondAuto : public QuadrupedRobot {
private:
    double rangeKm;

public:
    DiamondAuto(double rangeKm, const std::string& name);
    ~DiamondAuto();

    double getRangeKm() const { return rangeKm; }
};

class DiamondBoth : public DiamondCargo, public DiamondAuto {
public:
    DiamondBoth(double cargoWeight, double rangeKm, const std::string& name);
    ~DiamondBoth();
};
