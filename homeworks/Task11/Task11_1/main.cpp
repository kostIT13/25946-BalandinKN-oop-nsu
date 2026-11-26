#include "autonomous_robot.h"
#include "cargo_autonomous_robot.h"
#include "cargo_robot.h"
#include "diamond_problem.h"
#include "robot.h"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void sep(const std::string& title) {
    const size_t dashes = title.size() + 2 < 64 ? 64 - title.size() - 2 : 2;
    std::cout << "\n=== " << title << " " << std::string(dashes, '-') << "\n";
}

void alive(const std::string& where) {
    std::cout << "  >>> live QuadrupedRobot subobjects: "
              << QuadrupedRobot::getLiveCount() << "   (" << where << ")\n";
}

}

int main() {
    sep("1. Base class sets the instance name in its constructor");
    {
        QuadrupedRobot base("Atlas");
        std::cout << "  name=\"" << base.getName() << "\"\n";
        try {
            QuadrupedRobot noName("");
        } catch (const std::invalid_argument& e) {
            std::cout << "  empty name -> " << e.what() << "\n";
        }
    }

    sep("2. CargoRobot : public virtual QuadrupedRobot (+ cargo weight)");
    {
        CargoRobot cargo(35.0, "Mule");
        std::cout << "  name=\"" << cargo.getName()
                  << "\" cargo=" << cargo.getCargoWeight() << " kg\n";
        cargo.setCargoWeight(-4.0);
        std::cout << "  setCargoWeight(-4) normalizes to "
                  << cargo.getCargoWeight() << " kg\n";
    }

    sep("3. AutonomousRobot : public virtual QuadrupedRobot (+ range)");
    {
        AutonomousRobot auton(12.5, "Ghost");
        std::cout << "  name=\"" << auton.getName()
                  << "\" range=" << auton.getAutonomousRangeKm() << " km\n";
    }

    sep("4. CargoAutonomousRobot : CargoRobot + AutonomousRobot");
    QuadrupedRobot::resetBaseCtorCalls();
    {
        CargoAutonomousRobot both(20.0, 8.0, "Argus");
        both.describe();
        std::cout << "  base-class constructors called while building it: "
                  << QuadrupedRobot::getBaseCtorCalls()
                  << "  (1 virtual base, not 2)\n";
        alive("one CargoAutonomousRobot");
        std::cout << "  the same object seen through both branches: \""
                  << static_cast<CargoRobot&>(both).getName() << "\" / \""
                  << static_cast<AutonomousRobot&>(both).getName() << "\"\n";
    }

    sep("5.1 Diamond problem: the same hierarchy WITHOUT virtual base");
    QuadrupedRobot::resetBaseCtorCalls();
    {
        DiamondBoth broken(20.0, 8.0, "Argus");
        std::cout << "  base-class constructors called: "
                  << QuadrupedRobot::getBaseCtorCalls()
                  << "  (two QuadrupedRobot subobjects)\n";
        alive("ONE DiamondBoth object");
        std::cout << "  it has two names now: DiamondCargo branch=\""
                  << broken.DiamondCargo::getName()
                  << "\" DiamondAuto branch=\"" << broken.DiamondAuto::getName()
                  << "\"\n";

        sep("5.1.1 Consequence: the copies drift apart");
        broken.DiamondCargo::setName("Renamed-By-Cargo");
        std::cout << "  setName through one branch only:\n"
                  << "     DiamondCargo=\"" << broken.DiamondCargo::getName()
                  << "\"  DiamondAuto=\"" << broken.DiamondAuto::getName()
                  << "\"\n"
                  << "  an object of ONE class reports TWO different names —"
                     " the identity\n"
                  << "  of the base part is no longer unique\n";
        std::cout << "  sizeof(DiamondBoth)          = " << sizeof(DiamondBoth)
                  << " bytes\n";
        alive("before destroying the broken object");
    }

    QuadrupedRobot::resetBaseCtorCalls();
    {
        CargoAutonomousRobot solved(20.0, 8.0, "Argus");
        std::cout << "\n  sizeof(CargoAutonomousRobot) = "
                  << sizeof(CargoAutonomousRobot) << " bytes\n"
                  << "  base ctors called: "
                  << QuadrupedRobot::getBaseCtorCalls() << "\n";
        alive("ONE CargoAutonomousRobot object");
        solved.describe();
        std::cout << "  'is-a' conversion works for both bases and both see the"
                     " SAME name:\n     \""
                  << static_cast<CargoRobot&>(solved).getName() << "\" / \""
                  << static_cast<AutonomousRobot&>(solved).getName() << "\"\n";
    }

    sep("5.2 The solution in one line");
    std::cout << "  class CargoRobot : public virtual QuadrupedRobot {};\n"
                 "  class AutonomousRobot : public virtual QuadrupedRobot {};\n"
                 "  - exactly ONE shared base subobject in the diamond;\n"
                 "  - it is constructed by the MOST DERIVED class, which is why\n"
                 "    CargoAutonomousRobot names QuadrupedRobot(name) itself"
                 " and the\n"
                 "    name arguments of CargoRobot/AutonomousRobot are ignored"
                 " here;\n"
                 "  - unqualified r.getName() compiles again (see the note on\n"
                 "    diamond_ambiguity.cpp for the broken variant).\n";
    alive("end of main");
    return 0;
}
