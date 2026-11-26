#include "autonomous_cargo_quadruped_robot.h"
#include "cargo_quadruped_robot.h"
#include "quadruped_robot.h"
#include "robot.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

void sep(const std::string& title) {
    const size_t dashes = title.size() + 2 < 64 ? 64 - title.size() - 2 : 2;
    std::cout << "\n=== " << title << " " << std::string(dashes, '-') << "\n";
}

}

int main() {
    QuadrupedRobot quad("Quad", 100.0, 60);
    CargoQuadrupedRobot cargo("CargoQuad", 100.0, 200.0);
    AutonomousCargoQuadrupedRobot autoCargo("AutoCargoQuad", 100.0, 200.0, 60.0);

    sep("A. Abstract top of the hierarchy");
    std::cout << "  Chain of 4 classes, no multiple inheritance:\n"
                 "  Robot -> QuadrupedRobot -> CargoQuadrupedRobot ->\n"
                 "  AutonomousCargoQuadrupedRobot\n"
                 "  Robot::move() is pure virtual, so Robot has NO array"
                 " element:\n"
                 "  'Robot r;' does not compile (not_allowed_example.cpp)."
                 " Every\n"
                 "  concrete class of the chain is present in the array below,"
                 " and any\n"
                 "  element also IS a Robot — that is the only way to hold a"
                 " Robot here\n";

    std::vector<Robot*> fleet{&quad, &cargo, &autoCargo};

    sep("B. POLYMORPHIC: one array of Robot*, move() picked per object");
    for (Robot* r : fleet) std::cout << "  " << r->statusLine() << "\n";
    for (int step = 1; step <= 2; ++step) {
        std::cout << "  --- step " << step << " ---\n";
        for (Robot* r : fleet) r->move();
    }
    std::cout << "  ONE source line 'r->move()' produced three different\n"
                 "  implementations: gallop / loaded step / autonomous march\n";

    sep("C. NON-POLYMORPHIC 1: a non-virtual method in the same loop");
    for (Robot* r : fleet)
        std::cout << "  element points at " << r->className()
                  << ", r->gaitTag() = " << r->gaitTag() << "\n";
    std::cout << "  always 'base-gait': a non-virtual call is bound at compile\n"
                 "  time by the STATIC type (Robot*), the object is not asked\n";
    std::cout << "  the very same objects through their own type:\n"
              << "     quad.gaitTag()      = " << quad.gaitTag() << "\n"
              << "     cargo.gaitTag()     = " << cargo.gaitTag() << "\n"
              << "     autoCargo.gaitTag() = " << autoCargo.gaitTag() << "\n";

    sep("D. NON-POLYMORPHIC 2: array BY VALUE — object slicing");
    {
        QuadrupedRobot typed[2] = {quad, cargo};
        std::cout << "  typed[1] was built FROM a " << cargo.className()
                  << ",\n  after the copy it is a " << typed[1].className()
                  << ": only the QuadrupedRobot\n"
                     "  part of the object survived the copy\n";
        typed[1].move();
        std::cout << "  the cost was 1% (QuadrupedRobot::move), not the loaded\n"
                     "  cost of " << cargo.className() << ": the derived part —\n"
                     "  cargo weight and its move() — simply is not there\n";
    }

    sep("E. NON-POLYMORPHIC 3: own members are invisible to Robot*");
    std::cout << "  for (Robot* r : fleet) r->getCargoWeightKg() — does not\n"
                 "  compile: Robot* knows only the Robot interface.\n"
                 "  own members need the concrete type:\n"
              << "     cargo.getCargoWeightKg()        = "
              << cargo.getCargoWeightKg() << " kg\n"
              << "     autoCargo.getAutonomousRangeM() = "
              << autoCargo.getAutonomousRangeM() << " m\n"
              << "  a Robot* can be brought back with dynamic_cast:\n";
    for (Robot* r : fleet) {
        const CargoQuadrupedRobot* asCargo =
            dynamic_cast<const CargoQuadrupedRobot*>(r);
        std::cout << "     " << r->className() << " -> "
                  << (asCargo
                          ? "CargoQuadrupedRobot*, cargo=" +
                                std::to_string(asCargo->getCargoWeightKg()) +
                                " kg"
                          : "nullptr (not a cargo robot)")
                  << "\n";
    }

    sep("F. The 4th class differs even in its own state machine");
    for (int i = 0; i < 2; ++i) autoCargo.move();
    autoCargo.move();

    sep("G. After everything: one virtual interface, four depths of state");
    for (Robot* r : fleet) std::cout << "  " << r->statusLine() << "\n";
    return 0;
}
