#include "cargo_robot.h"

#include <stdexcept>
#include <utility>

const std::string CargoRobot::class_name = "CargoRobot";

double CargoRobot::normalizeCargo(double w, double maxW) {
    if (maxW <= 0.0)
        throw std::invalid_argument("max cargo weight must be positive");
    if (w < 0.0) return 0.0;
    if (w > maxW)
        throw std::overflow_error("cargo weight exceeds allowed maximum");
    return w;
}

void CargoRobot::printMsg(const std::string& what) const {
    std::cout << "  [" << class_name << " #" << getUid() << "] " << what
              << " | alive: " << getObjectCount() << "\n";
}

CargoRobot::CargoRobot()
    : QuadrupedRobot(), cargoWeight(0.0), maxCargoWeight(defaultMaxCargo()) {
    printMsg("default constructor");
}

CargoRobot::CargoRobot(const std::string& id, const std::string& name,
                       double battery, const std::string& command, double dist,
                       const double* last10Arr, double cargoWeight,
                       double maxCargoWeight)
    : QuadrupedRobot(id, name, battery, command, dist, last10Arr),
      cargoWeight(normalizeCargo(cargoWeight, maxCargoWeight)),
      maxCargoWeight(maxCargoWeight)
{
    printMsg("initialization constructor");
}

CargoRobot::CargoRobot(const CargoRobot& o)
    : QuadrupedRobot(o), cargoWeight(o.cargoWeight),
      maxCargoWeight(o.maxCargoWeight)
{
    printMsg("copy constructor (from #" + std::to_string(o.getUid()) + ")");
}

CargoRobot& CargoRobot::operator=(const CargoRobot& o) {
    if (this != &o) {
        QuadrupedRobot::operator=(o);
        cargoWeight = o.cargoWeight;
        maxCargoWeight = o.maxCargoWeight;
    }
    printMsg("assignment operator (from #" + std::to_string(o.getUid()) + ")");
    return *this;
}

CargoRobot::CargoRobot(CargoRobot&& o) noexcept
    : QuadrupedRobot(std::move(o)), cargoWeight(o.cargoWeight),
      maxCargoWeight(o.maxCargoWeight)
{
    o.cargoWeight = 0.0;
    printMsg("move constructor (from #" + std::to_string(o.getUid()) + ")");
}

CargoRobot& CargoRobot::operator=(CargoRobot&& o) noexcept {
    if (this != &o) {
        QuadrupedRobot::operator=(std::move(o));
        cargoWeight = o.cargoWeight;
        maxCargoWeight = o.maxCargoWeight;
        o.cargoWeight = 0.0;
        printMsg("move assignment (from #" + std::to_string(o.getUid()) + ")");
    }
    return *this;
}

CargoRobot::~CargoRobot() {
    printMsg("destructor");
}

void CargoRobot::show_weight() const {
    std::cout << "  " << class_name << " #" << getUid() << " (" << getName()
              << "): cargo = " << cargoWeight << " kg"
              << " / max " << maxCargoWeight << " kg\n";
}

void CargoRobot::printCargoRobot() const {
    printRobot();
    show_weight();
}
