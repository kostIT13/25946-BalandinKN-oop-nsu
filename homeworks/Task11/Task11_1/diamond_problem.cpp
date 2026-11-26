#include "diamond_problem.h"

DiamondCargo::DiamondCargo(double cargoWeight, const std::string& name)
    : QuadrupedRobot(name), cargoWeight(cargoWeight) {}

DiamondCargo::~DiamondCargo() {}

DiamondAuto::DiamondAuto(double rangeKm, const std::string& name)
    : QuadrupedRobot(name), rangeKm(rangeKm) {}

DiamondAuto::~DiamondAuto() {}

DiamondBoth::DiamondBoth(double cargoWeight, double rangeKm,
                         const std::string& name)
    : DiamondCargo(cargoWeight, name), DiamondAuto(rangeKm, name) {}

DiamondBoth::~DiamondBoth() {}
