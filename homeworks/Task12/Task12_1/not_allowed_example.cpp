#include "autonomous_cargo_quadruped_robot.h"
#include "quadruped_robot.h"
#include "robot.h"

void abstractClassCannotBeCreated() {
    Robot r("JustRobot");
    Robot* p = new Robot("OnHeap");
    (void)p;
}

class SilentRobot : public Robot {
public:
    SilentRobot() : Robot("Silent") {}
};

void derivedWithoutMoveStaysAbstract() {
    SilentRobot silent;
}

void protectedIsNotPublicApi() {
    Robot r("Name");
    r.normalizeBattery(50.0);
    r.logState("hack");
}

void privateFieldsAreClosedToEveryoneElse() {
    QuadrupedRobot q("Q");
    q.batteryLevel = -100.0;
    q.name = "rewritten";
}
