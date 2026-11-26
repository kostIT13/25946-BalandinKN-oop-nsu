#include "autonomous_cargo_robot.h"

#include <random>
#include <utility>

const std::string AutonomousCargoRobot::class_name = "AutonomousCargoRobot";

void AutonomousCargoRobot::printMsg(const std::string& what) const {
    std::cout << "  [" << class_name << " #" << getUid() << "] " << what
              << " | alive: " << getObjectCount() << "\n";
}

AutonomousCargoRobot::AutonomousCargoRobot()
    : CargoRobot(), autonomousMode(false) {
    printMsg("default constructor");
}

AutonomousCargoRobot::AutonomousCargoRobot(const std::string& id,
                                           const std::string& name,
                                           double battery,
                                           const std::string& command,
                                           double dist, const double* last10Arr,
                                           double cargoWeight,
                                           double maxCargoWeight,
                                           bool autonomousMode)
    : CargoRobot(id, name, battery, command, dist, last10Arr, cargoWeight,
                 maxCargoWeight),
      autonomousMode(autonomousMode)
{
    printMsg("initialization constructor");
}

AutonomousCargoRobot::AutonomousCargoRobot(const AutonomousCargoRobot& o)
    : CargoRobot(o), autonomousMode(o.autonomousMode)
{
    printMsg("copy constructor (from #" + std::to_string(o.getUid()) + ")");
}

AutonomousCargoRobot& AutonomousCargoRobot::operator=(
    const AutonomousCargoRobot& o) {
    if (this != &o) {
        CargoRobot::operator=(o);
        autonomousMode = o.autonomousMode;
    }
    printMsg("assignment operator (from #" + std::to_string(o.getUid()) + ")");
    return *this;
}

AutonomousCargoRobot::AutonomousCargoRobot(AutonomousCargoRobot&& o) noexcept
    : CargoRobot(std::move(o)), autonomousMode(o.autonomousMode)
{
    printMsg("move constructor (from #" + std::to_string(o.getUid()) + ")");
}

AutonomousCargoRobot& AutonomousCargoRobot::operator=(
    AutonomousCargoRobot&& o) noexcept {
    if (this != &o) {
        CargoRobot::operator=(std::move(o));
        autonomousMode = o.autonomousMode;
        printMsg("move assignment (from #" + std::to_string(o.getUid()) + ")");
    }
    return *this;
}

AutonomousCargoRobot::~AutonomousCargoRobot() {
    printMsg("destructor");
}

void AutonomousCargoRobot::reportMode() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> coin(0, 1);

    if (coin(gen) == 0) {
        std::cout << "no message (silent half of the cases)\n";
        return;
    }
    std::cout << "mode: " << modeName() << " (#" << getUid() << ")\n";
}

void AutonomousCargoRobot::printAutonomousRobot() const {
    printRobot();
    show_weight();
    std::cout << "  mode: " << modeName() << "\n";
}
