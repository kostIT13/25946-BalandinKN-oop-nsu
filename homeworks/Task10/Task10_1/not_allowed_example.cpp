#include "autonomous_cargo_robot.h"
#include "cargo_robot.h"
#include "robot.h"

void baseHasNoFriends() {
    QuadrupedRobot r;
    r.name = "hacked";
    r.batteryLevel = -5.0;
    r.last10 = nullptr;
    QuadrupedRobot::objectCount = 0;
    r.ensureBuffer();
}

void privateHidesCargoRobotInterface() {
    AutonomousCargoRobot a;
    CargoRobot& asCargo = a;
    QuadrupedRobot& asBase = a;
    a.printCargoRobot();
    a.getMaxCargoWeight();
    a.getId();
    a.QuadrupedRobot::printRobot();
    (void)asCargo;
    (void)asBase;
}

class CannotDeriveFromFinal final : public AutonomousCargoRobot {
public:
    void more() {}
};

void nobodyLiftsBaseMethodsThemselves() {
    AutonomousCargoRobot a;
    a.AutonomousCargoRobot::printRobot();
}
