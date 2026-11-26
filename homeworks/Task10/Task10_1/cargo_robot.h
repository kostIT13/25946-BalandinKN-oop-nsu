#pragma once
#include "robot.h"

#include <string>

class CargoRobot : public QuadrupedRobot {
private:
    double cargoWeight;
    double maxCargoWeight;

    static double normalizeCargo(double w, double maxW);
    void printMsg(const std::string& what) const;

public:
    static const std::string class_name;
    static double defaultMaxCargo() { return 50.0; }

    CargoRobot();
    CargoRobot(const std::string& id, const std::string& name, double battery,
               const std::string& command, double dist, const double* last10Arr,
               double cargoWeight, double maxCargoWeight = defaultMaxCargo());

    CargoRobot(const CargoRobot& o);
    CargoRobot& operator=(const CargoRobot& o);
    CargoRobot(CargoRobot&& o) noexcept;
    CargoRobot& operator=(CargoRobot&& o) noexcept;
    ~CargoRobot();

    void show_weight() const;
    void printCargoRobot() const;

    double getCargoWeight() const { return cargoWeight; }
    double getMaxCargoWeight() const { return maxCargoWeight; }
    void setCargoWeight(double w) { cargoWeight = normalizeCargo(w, maxCargoWeight); }
};
